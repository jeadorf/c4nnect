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
void stats_print(Board *b, Player p, Move *move, Statistics *stats);
#endif

void alphabeta_negamax(
        Board *b, Player p,
        float alpha, float beta,
        int depth, int max_depth,
        Move *move, Statistics *stats) {
    if (b->winner != NOBODY
            || board_full(b)
            || depth == max_depth) {
        move->value = (p == WHITE ? 1 : -1) * eval(b);
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
                        depth + 1, max_depth, move, stats);
                move->value *= -1;
                // Undo move
                board_undo(b, col);

                // Check for better move
                if (move->value > bestval || bestcol == -1) {
                    bestval = move->value;
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

        move->value = bestval;
        move->column = bestcol;
    }

    stats->visited_cnt++;
    stats->max_depth = max_depth;
    stats->winner_identified = (move->value <= -BONUS_WIN || move->value >= BONUS_WIN);
    if (depth > stats->reached_depth) {
        stats->reached_depth = depth;
    }
}

int search(Board *b, Player p) {
    // TODO: make one-level move history simpler
    Move *move = malloc(sizeof (Move));
    Move *last_move = malloc(sizeof (Move));
    Move *tmp;
    Statistics stats;
    stats_init(&stats);
    stats.cpu_time = clock();

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
    int iterations = 10 + (b->move_cnt * b->move_cnt) / (NUM_COLS * NUM_ROWS);
    for (int max_depth = 1; max_depth < iterations; max_depth++) {
        tmp = move;
        move = last_move;
        last_move = tmp;

        // TODO: Use results for move ordering or killer moves or something like this
        alphabeta_negamax(b, p, -FLT_MAX, FLT_MAX, 0, max_depth, move, &stats);
        // Check whether we have found a winning move. If we have there is no point
        // in looking for more complicated ways to victory.
        if (stats.winner_identified) {
            break;
        }
    }

    stats.cpu_time = clock() - stats.cpu_time;

    // Defer defeats that are unavoidable. The computer should at least not to
    // lose in the next move even if the computer sees that he will lose
    // whatever moves he plays (hey, that is not correct, he will only lose
    // if the opponent always plays correctly). In the case the lookahead value
    // is greater than the value returned by the deep search then the lookahead
    // simply did not discover the threats hidden some plies ahead. And it
    // therefore just avoids being defeated in the next two steps.
    if (stats.winner_identified && move->value <= -BONUS_WIN) {
        tmp = move;
        move = last_move;
        last_move = tmp;
        stats.defeat_deferred = true;
    }

#ifdef DEBUG
    stats_print(b, p, move, &stats);
#endif
    int col = move->column;
    free(last_move);
    free(move);
    return col;
}

#ifdef DEBUG

void stats_print(Board *b, Player p, Move *move, Statistics *stats) {
    // Print statistics
    board_print(b);
    printf("%18s : %d\n", "Move", move->column);
    if (move->value >= BONUS_WIN) {
        printf("%18s : %s\n", "Value", p == WHITE ? "White will win" : "Black will win");
    } else if (move->value <= -BONUS_WIN) {
        printf("%18s : %s\n", "Value", other(p) == WHITE ? "White will win" : "Black will win");
    } else {
        printf("%18s : %.1f\n", "Value", move->value);
    }
    printf("%18s : %d\n", "Winner identified", stats->winner_identified);
    printf("%18s : %d\n", "Defeat deferred", stats->defeat_deferred);
    printf("%18s : %d\n", "Maximum depth", stats->max_depth);
    printf("%18s : %d\n", "Reached depth", stats->reached_depth);
    printf("%18s : %ld\n", "Evaluations", stats->eval_cnt);
    printf("%18s : %ld\n", "Positions", stats->visited_cnt);
    printf("%18s : %ld\n", "Alpha-Beta cuts:", stats->abcut_cnt);
    printf("%18s : %ld\n", "Win cuts:", stats->wincut_cnt);
    printf("%18s : %d ms\n", "CPU time", (int) (stats->cpu_time / (CLOCKS_PER_SEC / 1000)));
    putchar('\n');
}
#endif
