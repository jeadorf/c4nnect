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
void stats_print(Board *b, Player p, SearchRecord *rec);
#endif

void alphabeta_negamax(
        Board *b, Player p,
        float alpha, float beta,
        int depth, int max_depth,
        SearchRecord *rec) {
    if (b->winner != NOBODY
            || board_full(b)
            || depth == max_depth) {
        rec->rating = (p == WHITE ? 1 : -1) * eval(b);
        rec->eval_cnt++;
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
                        depth + 1, max_depth, rec);
                rec->rating *= -1;
                // Undo move
                board_undo(b, col);

                // Check for better move
                if (rec->rating > bestval || bestcol == -1) {
                    bestval = rec->rating;
                    bestcol = col;
                }
                // Check for a beta cutoff
                if (bestval >= beta) {
                    rec->abcut_cnt++;
                    break;
                }
                // Check for a win situation, there is no need to look for a
                // better move than one that already wins.
                if (bestval >= BONUS_WIN) {
                    rec->wincut_cnt++;
                    break;
                }
            }
        }

        rec->rating = bestval;
        rec->move = bestcol;
    }

    rec->visited_cnt++;
    rec->max_depth = max_depth;
    rec->winner_identified = (rec->rating <= -BONUS_WIN || rec->rating >= BONUS_WIN);
    if (depth > rec->reached_depth) {
        rec->reached_depth = depth;
    }
}

int searchm(Board *b, Player p) {
    SearchRecord rec;
    searchrecord_init(&rec);
    search(b, p, &rec);
    return rec.move;
}

void search(Board *b, Player p, SearchRecord *rec) {
    int last_move;
    float last_rating;
    
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
    int iterations = 10 + (b->move_cnt * b->move_cnt) / (NUM_COLS * NUM_ROWS);
    for (int max_depth = 1; max_depth < iterations; max_depth++) {
        last_move = rec->move;
        last_rating = rec->rating;
        // TODO: Use results for move ordering or killer moves or something like this
        alphabeta_negamax(b, p, -FLT_MAX, FLT_MAX, 0, max_depth, rec);
        // Check whether we have found a winning move. If we have there is no point
        // in looking for more complicated ways to victory.
        if (rec->winner_identified) {
            break;
        }
    }

    rec->cpu_time = clock() - rec->cpu_time;

    // Defer defeats that are unavoidable. The computer should at least not to
    // lose in the next move even if the computer sees that he will lose
    // whatever moves he plays (hey, that is not correct, he will only lose
    // if the opponent always plays correctly). In the case the lookahead value
    // is greater than the value returned by the deep search then the lookahead
    // simply did not discover the threats hidden some plies ahead. And it
    // therefore just avoids being defeated in the next two steps.
    if (rec->winner_identified && rec->rating <= -BONUS_WIN) {
        rec->move = last_move;
        rec->rating = last_rating;
        rec->defeat_deferred = true;
    }

#ifdef DEBUG
    stats_print(b, p, rec);
#endif
}

#ifdef DEBUG

void stats_print(Board *b, Player p, SearchRecord *rec) {
    // Print statistics
    board_print(b);
    printf("%18s : %d\n", "Move", rec->move);
    if (rec->rating >= BONUS_WIN) {
        printf("%18s : %s\n", "Rating", p == WHITE ? "White will win" : "Black will win");
    } else if (rec->rating <= -BONUS_WIN) {
        printf("%18s : %s\n", "Rating", other(p) == WHITE ? "White will win" : "Black will win");
    } else {
        printf("%18s : %.1f\n", "Rating", rec->rating);
    }
    printf("%18s : %d\n", "Winner identified", rec->winner_identified);
    printf("%18s : %d\n", "Defeat deferred", rec->defeat_deferred);
    printf("%18s : %d\n", "Maximum depth", rec->max_depth);
    printf("%18s : %d\n", "Reached depth", rec->reached_depth);
    printf("%18s : %ld\n", "Evaluations", rec->eval_cnt);
    printf("%18s : %ld\n", "Positions", rec->visited_cnt);
    printf("%18s : %ld\n", "Alpha-Beta cuts:", rec->abcut_cnt);
    printf("%18s : %ld\n", "Win cuts:", rec->wincut_cnt);
    printf("%18s : %d ms\n", "CPU time", (int) (rec->cpu_time / (CLOCKS_PER_SEC / 1000)));
    putchar('\n');
}
#endif
