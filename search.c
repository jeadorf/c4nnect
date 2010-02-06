#include <math.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "search.h"
#include "stats.h"
#include "board.h"
#include "eval.h"
#include "util.h"
#include "ttable.h"

#ifdef DEBUG
void stats_print(Board *b, SearchRecord *rec);
#endif

extern TTEntry ttable[];

// Simple move ordering, start with checking the moves in the center and
// then circle to the outer columns

void generate_moves(SearchRecord *rec, int depth, int8_t *moves) {
    int8_t i = 0, k = 0, col = 0;
    if (depth < rec->pv.length) {
        moves[0] = rec->pv.moves[depth];
        i++;
    }
    while (k < NUM_COLS) {
        col = (NUM_COLS / 2) + (k % 2 == 0 ? 1 : -1) * (k + 1) / 2;
        if (i == 0 || col != moves[0]) {
            moves[i] = col;
            i++;
        }
        k++;
    }
}

void alphabeta_negamax(
        Board *b,
        float alpha, float beta,
        int8_t depth, int8_t max_depth, bool ttcuts_enabled,
        SearchRecord * rec) {
#ifndef DISABLE_TTABLE
    uint32_t hash = b->code % TTABLE_SIZE;
    TTEntry *ttentry = &(ttable[hash]);
#endif

    if (b->winner != NOBODY
            || board_full(b)
            || depth == max_depth) {
        rec->rating = (b->turn == WHITE ? 1 : -1) * eval(b);
        rec->winner_identified = (rec->rating <= ALPHA_MIN || rec->rating >= BETA_MAX);
        rec->eval_cnt++;
        rec->pv.length = 0;
#ifndef DISABLE_TTABLE
    } else if (ttcuts_enabled && ttentry->code == b->code && depth > 0) {
        // The transposition entry does not contain information about the move
        // that lead to the position, so we cannot check the transposition table
        // at the root of the search tree.
        rec->rating = ttentry->rating;
        rec->winner_identified = true;
        rec->ttcut_cnt++;
        rec->pv.length = 0;
#endif
    } else {
#ifndef DISABLE_TTABLE
        if (ttentry->code != b->code && ttentry->code != 0) {
            rec->ttrcoll_cnt++;
        }
#endif

        float bestval = alpha;
        int bestcol = -1;
        Variation pv = rec->pv;

        // FIXME: There might be invalid boards that are not classified
        //        correctly because of transposition tables!
        int8_t moves[NUM_COLS];
        generate_moves(rec, depth, moves);
        for (int8_t i = 0; i < NUM_COLS; i++) {
            int8_t col = moves[i];
            if (!board_column_full(b, col)) {
                // Make move
                board_put(b, col);

                // Search subposition
                alphabeta_negamax(b, -beta, -bestval, depth + 1, max_depth, ttcuts_enabled, rec);
                rec->rating *= -1;

                // Undo move
                board_undo(b, col);

                if (rec->rating > bestval || bestcol == -1) {
                    // Found a move that is either better than all other moves
                    bestval = rec->rating;
                    bestcol = col;
                    // Construct principal variation. Append the PV of the subtree
                    // to the PV that led to this node.
                    pv.moves[0] = col;
                    memcpy(pv.moves + 1, rec->pv.moves, rec->pv.length * sizeof (col));
                    pv.length = rec->pv.length + 1;
                }

#ifndef DISABLE_ABCUTS
                // Check for a beta cutoff
                if (bestval >= beta) {
                    rec->abcut_cnt++;
                    break;
                }
#endif
            }
        }

        rec->pv = pv;
        rec->rating = bestval;
        rec->winner_identified = (rec->rating <= ALPHA_MIN || rec->rating >= BETA_MAX);
#ifndef DISABLE_TTABLE
        if (rec->winner_identified) {
            ttentry->code = b->code;
            ttentry->rating = rec->rating;
            // TODO: this might count some of the entries twice?
            rec->ttadd_cnt++;
        }
#endif
    }

    rec->visited_cnt++;
    rec->max_depth = max_depth;
    if (depth > rec->reached_depth) {
        rec->reached_depth = depth;
    }
}

// TODO: use principle variation from last search! Might check whether we
//      are still in this variation!

int8_t searchm(Board * b) {
    SearchRecord rec;
    searchrecord_init(&rec);
    search(b, &rec);
    return rec.pv.moves[0];
}

void search(Board *b, SearchRecord * rec) {
    rec->cpu_time = clock();

    // The maximum numbers of iterations (or plies) the search will do depends
    // on the current stage of the game.
    // If the game is at its very beginning, there is no use in doing a lot of
    // research if no forced variation can be found. In later stages, the number
    // of possible moves decreases with the board getting fuller and fuller.
    // Thus, it does not hurt to increase the search depth at later times in the
    // game.
    // The iterative approach implies that max_depth will never exceed reached_depth
    int8_t iterations = 11 + (b->move_cnt * b->move_cnt) * 1.25 / (NUM_COLS * NUM_ROWS);
    // Maximum iterations should not exceed number of free slots
    // TODO: remove magic number 42
    if (iterations > (42 - b->move_cnt)) {
        iterations = 43 - b->move_cnt;
    }
    for (int8_t max_depth = 1; max_depth < iterations; max_depth++) {
        alphabeta_negamax(b, ALPHA_MIN, BETA_MAX, 0, max_depth, true, rec);
        if (rec->winner_identified) {
            // Defer defeats that are unavoidable. The computer should at least not to
            // lose in the next move even if the computer sees that he will lose against
            // the perfect-playing opponent. Deferring defeats does only work without
            // transposition tables that are not cleared between subsequent searches.
            if (rec->rating <= ALPHA_MIN) {
                int8_t reached_depth = rec->reached_depth;
                SearchRecord defrec;
                searchrecord_init(&defrec);
                alphabeta_negamax(b, ALPHA_MIN, BETA_MAX, 0,  max_depth - 1, false, &defrec);
                // Merge search results. This is not a really satisfying solution
                // but it saves some difficulties in handling two partial search
                // results
                rec->defeat_deferred = true;
                rec->pv = defrec.pv;
                rec->visited_cnt += defrec.visited_cnt;
                rec->eval_cnt += defrec.eval_cnt;
                rec->abcut_cnt += defrec.abcut_cnt;
                rec->ttcut_cnt += defrec.ttcut_cnt;
                rec->ttadd_cnt += defrec.ttadd_cnt;
                rec->ttrcoll_cnt += defrec.ttrcoll_cnt;
                rec->reached_depth = reached_depth;
            }
            break;
        }
    }
    rec->cpu_time = clock() - rec->cpu_time;
    rec->ttcharge = ttable_charge();
}
