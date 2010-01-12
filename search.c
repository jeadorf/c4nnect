#include <float.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "search.h"
#include "board.h"
#include "eval.h"
#include "util.h"

void alphabeta_negamax(
        Board *b, Player p,
        float alpha, float beta,
        int depth, int max_depth,
        Move *result) {
    if (b->winner != NOBODY
            || board_full(b)
            || depth == max_depth) {
        result->value = (p == WHITE ? 1 : -1) * eval(b);
    } else {
        float bestval = alpha;
        int bestcol = -1;
        for (int col = 0; col < NUM_COLS; col++) {
            if (!board_column_full(b, col)) {
                // Make move
                board_put(b, p, col);

                // Search subposition
                alphabeta_negamax(b, other(p), -beta, -bestval,
                                  depth + 1, max_depth, result);
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
                    break;
                }
            }
        }

        result->value = bestval;
        result->column = bestcol;
    }
    /*
        board_printd(b, depth);
        int i;
        for (i = 0; i < 4 * depth; i++) putchar(' ');
        printf("player %d, ply %d\n", p, depth);
        for (i = 0; i < 4 * depth; i++) putchar(' ');
        printf("value %.2f\n", result->value);
        for (i = 0; i < 4 * depth; i++) putchar(' ');
        printf("move %d\n\n\n", result->column);
     */
}

int search(Board *b, Player p) {
    Move lookahead;
    // Look-ahead for quick ways to victory. This makes the computer's moves
    // much more predictible if it is on the road to victory. Otherwise the
    // computer might create additional threats that ultimately lead to victory
    // instead of winning by simply completing a row.
    alphabeta_negamax(b, p, -FLT_MAX, FLT_MAX, 0, 2, &lookahead);
    // Check whether we have found a winning move. If we have there is no point
    // in looking for more complicated ways to victory.
    if (lookahead.value > BONUS_WIN) {
        return lookahead.column;
    }

    Move deepsearch;
    // Now perform a full-scale deep alphabeta search
    alphabeta_negamax(b, p, -FLT_MAX, FLT_MAX, 0, 9, &deepsearch);
    // Defer defeats that are unavoidable. The computer should at least not to
    // lose in the next move even if the computer sees that he will lose
    // whatever moves he plays (hey, that is not correct, he will only lose
    // if the opponent always plays correctly).
    if (deepsearch.value > lookahead.value) {
        return deepsearch.column;
    } else {
        return lookahead.column;
    }
}
