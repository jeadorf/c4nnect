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
void stats_print(Board *b, Variation *var, SearchRecord *rec);
#endif

extern TTEntry ttable[];

// Simple move ordering, first walk along the principal variation then start
// with checking the moves in the center and then circle to the outer columns

void generate_moves(Variation *var, int depth, int8_t *moves) {
    int8_t i = 0, k = 0, col = 0;
    if (depth < var->length) {
        moves[0] = var->moves[depth];
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
        int8_t depth, int8_t max_depth,
        bool ttcuts_enabled, bool defer_defeat,
        Variation *var, SearchRecord *rec) {
#ifndef DISABLE_TTABLE
    uint32_t hash = b->code % TTABLE_SIZE;
    TTEntry *ttentry = &(ttable[hash]);
#endif

    if (b->winner != NOBODY
            || board_full(b)
            || depth == max_depth) {
        if (depth == 0) {
            handle_error("cannot search for a move in a finished game!");
        }
        var->rating = (b->turn == WHITE ? 1 : -1) * eval(b, defer_defeat);
        var->length = 0;
        rec->eval_cnt++;
#ifndef DISABLE_TTABLE
    } else if (ttcuts_enabled && ttentry->code == b->code && depth > 0) {
        // The transposition entry does not contain information about the move
        // that lead to the position, so we cannot check the transposition table
        // at the root of the search tree.
        var->rating = ttentry->rating;
        var->length = 0;
        rec->ttcut_cnt++;
#endif
    } else {
#ifndef DISABLE_TTABLE
        if (ttcuts_enabled && ttentry->code != b->code && ttentry->code != 0) {
            rec->ttrcoll_cnt++;
        }
#endif

        float bestval = alpha;
        int bestcol = -1;
        Variation pv = *var;

        // FIXME: There might be invalid boards that are not classified
        //        correctly because of transposition tables!??
        int8_t moves[NUM_COLS];
        generate_moves(var, depth, moves);
        for (int8_t i = 0; i < NUM_COLS; i++) {
            int8_t col = moves[i];
            if (!board_column_full(b, col)) {
                // Make move
                // TODO: check whether undo mechanism does any good
                board_put(b, col);

                // Search subposition
                alphabeta_negamax(b, -beta, -bestval, depth + 1, max_depth,
                        ttcuts_enabled, defer_defeat, var, rec);
                var->rating *= -1;

                // Undo move
                board_undo(b, col);

                // TODO: check comparison with zero
                if (var->rating > bestval || bestcol == -1) {
                    // Found a move that is either better than all other moves
                    bestval = var->rating;
                    bestcol = col;
                    // Construct principal variation. Append the PV of the subtree
                    // to the PV that led to this node.
                    pv.moves[0] = col;
                    memcpy(pv.moves + 1, var->moves, var->length * sizeof (col));
                    pv.length = var->length + 1;
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

        *var = pv;
        var->rating = bestval;
#ifndef DISABLE_TTABLE
        if (winner_identified(var->rating)) {
            ttentry->code = b->code;
            ttentry->rating = var->rating;
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

int8_t searchm(Board * b) {
    Variation var;
    variation_init(&var);
    SearchRecord rec;
    searchrecord_init(&rec);
    search(b, &var, &rec);
    return var.moves[0];
}

void search(Board *b, Variation *var, SearchRecord *rec) {
    clock_t start_time = clock();
    rec->cpu_time = start_time;
    // The iterative approach implies that max_depth will never exceed
    // reached_depth.
    float time[43] = {0}; // nodes = interval count + 1
    int8_t max_depth = 0;
    float time_est = 0;
    do {
        max_depth++;
        variation_init(var);
        alphabeta_negamax(b, ALPHA_MIN, BETA_MAX, 0, max_depth, true, false, var, rec);

        if (winner_identified(var->rating)) {
            // Defer defeats that are unavoidable. The computer should at least not to
            // lose in the next move even if the computer sees that he will lose against
            // the perfect-playing opponent. Deferring defeats does only work without
            // transposition tables for they are not cleared between subsequent
            // searches.
            if (var->rating <= ALPHA_MIN) {
                int8_t reached_depth = rec->reached_depth;
                Variation defvar;
                variation_init(&defvar);
                SearchRecord defrec;
                searchrecord_init(&defrec);
                alphabeta_negamax(b, ALPHA_MIN_DEFEAT, BETA_MAX_DEFEAT,
                        0, max_depth - 1, false, true, &defvar, &defrec);
                // Merge search results. This is not a really satisfying solution
                // but it saves some difficulties in handling two partial search
                // results
                *var = defvar;
                var->rating = ALPHA_MIN;

                // TODO: this looks like a workaround
                rec->defeat_deferred = true;
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

#if DEBUG
        printf("estimated time %dms", (int) time_est);
#endif
        time[max_depth] = (clock() - start_time) / (CLOCKS_PER_SEC / 1000);
#if DEBUG
        printf(", actual time: %dms\n", (int) time[max_depth]);
#endif

        // Estimate the accumulated search time after the next iteration. This
        // extrapolation uses simple exponential regression b*a^t defined by
        // t=0, t=1 and evaluated at t=2.
        if (time[max_depth - 1] > 0 && time[max_depth] > 0) {
            time_est = time[max_depth] * time[max_depth] / time[max_depth - 1];
        } else {
            time_est = 0;
        }
    } while (time_est < TIME_LIMIT_PER_PLY && time[max_depth] < TIME_LIMIT_PER_PLY && max_depth < 42 - b->move_cnt);

    rec->cpu_time = clock() - rec->cpu_time;
    rec->on_time = (rec->cpu_time / (CLOCKS_PER_SEC / 1000)) < TIME_LIMIT_PER_PLY;
    rec->ttcharge = ttable_charge();
}
