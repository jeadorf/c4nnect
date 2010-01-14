#include <float.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "search.h"
#include "stats.h"
#include "board.h"
#include "eval.h"
#include "util.h"

#ifdef DEBUG
void stats_print(
        Board *b, Player p, int actual_move,
        Statistics *las, Statistics *dss,
        Move *lam, Move *dsm);
#endif

void alphabeta_negamax(
        Board *b, Player p,
        float alpha, float beta,
        int depth, int max_depth,
        Move *result, Statistics *stats) {
    if (depth == 0) {
        stats->cpu_time = clock();
    }
    if (b->winner != NOBODY
            || board_full(b)
            || depth == max_depth) {
        result->value = (p == WHITE ? 1 : -1) * eval(b);
        stats->eval_cnt++;
    } else {
        float bestval = alpha;
        int bestcol = -1;
        // Simple move ordering, start with checking the moves in the center and
        // then circle to the outer columns
        // TODO: find better move ordering such that we can use negascout (PVS) here
        for (int i = 0, s = -1, col = NUM_COLS / 2; i < NUM_COLS; i++, s *= -1, col += s * i) {
            if (!board_column_full(b, col)) {
                // Make move
                board_put(b, p, col);

                // Search subposition
                alphabeta_negamax(b, other(p), -beta, -bestval,
                        depth + 1, max_depth, result, stats);
                result->value *= -1;
                // Undo move
                board_undo(b, col);

                // Check for better move
                if (result->value > bestval || bestcol == -1) {
                    bestval = result->value;
                    bestcol = col;
                }
                // Check for a beta cutoff
                if (bestval >= beta) {
                    stats->abcut_cnt++;
                    break;
                }
                // Check for a win situation, there is no need to look for a
                // better move than one that already wins.
                if (bestval >= BONUS_WIN) {
                    stats->wincut_cnt++;
                    break;
                }
            }
        }

        result->value = bestval;
        result->column = bestcol;
    }

    if (depth == 0) {
        stats->max_depth = max_depth;
        stats->cpu_time = clock() - stats->cpu_time;
        stats->forced_variation = (result->value <= -BONUS_WIN || result->value >= BONUS_WIN);
    }
    if (depth > stats->reached_depth) {
        stats->reached_depth = depth;
    }
    stats->visited_cnt++;
}

int search(Board *b, Player p) {
    Move lookahead;
    Statistics lookahead_stats;
    stats_init(&lookahead_stats);

    // Look-ahead for quick ways to victory. This makes the computer's moves
    // much more predictible if it is on the road to victory. Otherwise the
    // computer might create additional threats that ultimately lead to victory
    // instead of winning by simply completing a row.
    alphabeta_negamax(b, p, -FLT_MAX, FLT_MAX, 0, 2, &lookahead, &lookahead_stats);
    // Check whether we have found a winning move. If we have there is no point
    // in looking for more complicated ways to victory.
    if (lookahead.value >= BONUS_WIN) {
        // TODO: preliminary return will skip statistics
        return lookahead.column;
    }
    // TODO: The first lookahead search is no longer necessary since we use
    // iterative deepening search that includes this lookahead search.
    Move deepsearch;
    Statistics deepsearch_stats;
    stats_init(&deepsearch_stats);

    // Now perform an iterative-deepening full-scale deep alphabeta search. The
    // maximum numbers of iterations (or plies) the search will do depends on the
    // current stage of the game. If the game is at its very beginning, there is
    // no use in doing a lot of research if no forced variation can be found. In
    // later stages, the number of possible moves decreases with the board
    // getting fuller and fuller. Thus, it does not hurt to increase the search
    // depth at later times in the game.
    int iterations = 10 + (b->move_cnt * b->move_cnt) / (NUM_COLS * NUM_ROWS);
    for (int max_depth = 0; max_depth < iterations; max_depth++) {
        // TODO: Fix statistics
        // TODO: Use results for move ordering or killer moves or something like this
        alphabeta_negamax(b, p, -FLT_MAX, FLT_MAX, 0, max_depth, &deepsearch, &deepsearch_stats);
        if (deepsearch_stats.forced_variation) {
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
    int move;
    if (deepsearch.value <= lookahead.value && deepsearch.value <= -BONUS_WIN) {
        move = lookahead.column;
    } else {
        move = deepsearch.column;
    }

#ifdef DEBUG
    stats_print(b, p, move, &lookahead_stats, &deepsearch_stats, &lookahead, &deepsearch);
#endif
    return move;
}

#ifdef DEBUG
void stats_print(
        Board *b, Player p, int actual_move,
        Statistics *las, Statistics *dss,
        Move *lam, Move *dsm) {
    // Print statistics
    board_print(b);
    printf("%16s : %d\n", "Actual move", actual_move);
    printf("%16s : %d\n", "Move", dsm->column);
    if (dsm->value >= BONUS_WIN) {
        printf("%16s : %s\n", "Value", p == WHITE ? "White will win" : "Black will win");
    } else if (dsm->value <= -BONUS_WIN) {
        printf("%16s : %s\n", "Value", other(p) == WHITE ? "White will win" : "Black will win");
    } else {
        printf("%16s : %.1f\n", "Value", dsm->value);
    }
    printf("%16s : %d\n", "Forced variation", dss->forced_variation);
    printf("%16s : %d\n", "Lookahead move", lam->column);
    if (lam->value >= BONUS_WIN) {
        printf("%16s : %s\n", "Lookahead value", p == WHITE ? "White will win" : "Black will win");
    } else if (lam->value <= -BONUS_WIN) {
        printf("%16s : %s\n", "Lookahead value", other(p) == WHITE ? "White will win" : "Black will win");
    } else {
        printf("%16s : %.1f\n", "Lookahead value", lam->value);
    }
    printf("%16s : %d\n", "Maximum depth", dss->max_depth);
    printf("%16s : %d\n", "Reached depth", dss->reached_depth);
    printf("%16s : %ld\n", "Evaluations", dss->eval_cnt + las->eval_cnt);
    printf("%16s : %ld\n", "Positions", dss->visited_cnt + las->visited_cnt);
    printf("%16s : %ld\n", "Alpha-Beta cuts:", dss->abcut_cnt + dss->abcut_cnt);
    printf("%16s : %ld\n", "Win cuts:", dss->wincut_cnt + dss->wincut_cnt);
    // printf("%16s : %d\n", "Forced variation", dss->forced_variation);
    printf("%16s : %d ms\n", "CPU time", (int) ((dss->cpu_time + las->cpu_time) / (CLOCKS_PER_SEC / 1000)));
}
#endif
