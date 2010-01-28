#include <float.h>
#include <malloc.h>
#include <math.h>
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

void alphabeta_negamax(
        Board *b,
        float alpha, float beta,
        int8_t depth, int8_t max_depth,
        SearchRecord *rec) {
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
#ifndef DISABLE_TTABLE
    } else if (ttentry->code == b->code && depth > 0) {
        // The transposition entry does not contain information about the move
        // that lead to the position, so we cannot check the transposition table
        // at the root of the search tree.
        rec->rating = ttentry->rating;
        rec->winner_identified = true;
        rec->ttcut_cnt++;
#endif
    } else {
#ifndef DISABLE_TTABLE
        if (ttentry->code != b->code && ttentry->code != 0) {
            rec->ttrcoll_cnt++;
        }
#endif

        float bestval = alpha;
        int bestcol = -1;
        // Simple move ordering, start with checking the moves in the center and
        // then circle to the outer columns
        // TODO: find better move ordering such that we can use negascout (PVS) here
        // it's probably best to evaluate the column first where the opponent has
        // put his piece - because it's likely to influence the game locally and
        // threats are detected more easily. Secondly, we can use information from
        // previous searches or even create a killer move table or some other
        // history heuristic
        for (int8_t i = 0, s = -1, col = NUM_COLS / 2; i < NUM_COLS; i++, s *= -1, col += s * i) {
            if (!board_column_full(b, col)) {
                // Make move
                board_put(b, col);

                // Search subposition
                alphabeta_negamax(b, -beta, -bestval, depth + 1, max_depth, rec);
                rec->rating *= -1;
                // Undo move
                board_undo(b, col);

                // Check for better move
                if (rec->rating > bestval || bestcol == -1) {
                    bestval = rec->rating;
                    bestcol = col;
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

        rec->rating = bestval;
        rec->move = bestcol;
        rec->winner_identified = (rec->rating <= ALPHA_MIN || rec->rating >= BETA_MAX);
#ifndef DISABLE_TTABLE
        if (rec->winner_identified) {
            ttentry->code = b->code;
            ttentry->rating = rec->rating;
        }
#endif
    }

    rec->visited_cnt++;
    rec->max_depth = max_depth;
    if (depth > rec->reached_depth) {
        rec->reached_depth = depth;
    }

#ifdef DDEBUG
    board_printd(b, depth);
    stats_print(b, p, rec);
#endif
}

int8_t searchm(Board *b) {
    SearchRecord rec;
    searchrecord_init(&rec);
    search(b, &rec);
    return rec.move;
}

void search(Board *b, SearchRecord *rec) {
    int8_t last_move = 0;
    rec->cpu_time = clock();

    // Now perform an iterative-deepening alphabeta search. First, look ahead
    // for quick ways to victory. This makes the computer's moves much more
    // predictible if it is on the road to victory otherwise the computer might
    // create additional threats that will later lead to victory instead of
    // winning by simply completing a row. The maximum numbers of iterations
    // (or plies) the search will do depends on the current stage of the game.
    // If the game is at its very beginning, there is no use in doing a lot of
    // research if no forced variation can be found. In later stages, the number
    // of possible moves decreases with the board getting fuller and fuller.
    // Thus, it does not hurt to increase the search depth at later times in the
    // game.
    // The iterative approach implies that max_depth will never exceed reached_depth
    int8_t iterations = 10 + (b->move_cnt * b->move_cnt) * 1.1 / (NUM_COLS * NUM_ROWS);
    for (int8_t max_depth = 1; max_depth < iterations; max_depth++) {
        last_move = rec->move;
        // TODO: Use results for move ordering or killer moves or something like this
        alphabeta_negamax(b, ALPHA_MIN, BETA_MAX, 0, max_depth, rec);
        // Check whether we have found a winning move. If we have there is no point
        // in looking for more complicated ways to victory.
        if (rec->winner_identified) {
            break;
        }
    }

    // Defer defeats that are unavoidable. The computer should at least not to
    // lose in the next move even if the computer sees that he will lose
    // whatever moves he plays (hey, that is not correct, he will only lose
    // if the opponent always plays correctly). In the case the lookahead value
    // is greater than the value returned by the deep search then the lookahead
    // simply did not discover the threats hidden some plies ahead. And it
    // therefore just avoids being defeated in the next two steps.
    if (rec->winner_identified && rec->rating <= ALPHA_MIN) {
        rec->move = last_move;
        rec->defeat_deferred = true;
    }

    rec->cpu_time = clock() - rec->cpu_time;
#ifdef DEBUG
    stats_print(b, rec);
#endif
}

#ifdef DEBUG

void stats_print(Board *b, SearchRecord *rec) {
    // Print statistics
    board_print(b);
    printf("%18s : %d\n", "Move", rec->move);
    if (rec->rating >= BETA_MAX) {
        printf("%18s : %s\n", "Rating", b->turn == WHITE ? "White will win" : "Black will win");
    } else if (rec->rating <= ALPHA_MIN) {
        printf("%18s : %s\n", "Rating", other(b->turn) == WHITE ? "White will win" : "Black will win");
    } else {
        printf("%18s : %.1f\n", "Rating", rec->rating);
    }
    printf("%18s : %d\n", "Winner identified", rec->winner_identified);
    printf("%18s : %d\n", "Defeat deferred", rec->defeat_deferred);
    printf("%18s : %d\n", "Maximum depth", rec->max_depth);
    printf("%18s : %d\n", "Reached depth", rec->reached_depth);
    printf("%18s : %ld\n", "Evaluations", rec->eval_cnt);
    printf("%18s : %ld\n", "Positions", rec->visited_cnt);
    printf("%18s : %ld\n", "Alpha-Beta cuts", rec->abcut_cnt);
    printf("%18s : %ld\n", "TTable cuts", rec->ttcut_cnt);
    printf("%18s : %ld\n", "TTable read colls.", rec->ttrcoll_cnt);
    printf("%18s : %d ms\n", "CPU time", (int) (rec->cpu_time / (CLOCKS_PER_SEC / 1000)));
    printf("%18s : 0x%.16lX\n", "Board", board_encode(b));
    putchar('\n');
}
#endif
