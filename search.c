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
                alphabeta_negamax(b, p == WHITE ? BLACK : WHITE,
                        -beta, -bestval, depth + 1, max_depth, result);
                result->value *= -1;
                // Undo move
                board_undo(b, col);

                // Check for better move
                if (result->value > bestval) {
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
    Move res;
    alphabeta_negamax(b, p, -FLT_MAX, FLT_MAX, 0, 4, &res);
    return res.column;
}
