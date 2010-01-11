#include <float.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "search.h"
#include "board.h"
#include "eval.h"

static int selected_column;
static float minimax_value;

struct Move {
    int column;
    float value;
};

typedef struct Move Move;

static void alphabeta_negamax(
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
                // Check for better move
                if (bestval < -result->value) {
                    bestval = -result->value;
                    bestcol = col;
                }
                // Undo move
                board_undo(b, col);
                // Check for a beta cutoff
                if (bestval >= beta) {
                    break;
                }
            }
        }

        result->value = bestval;
        result->column = bestcol;
    }
}

int search(Board b, Player p) {
    //Board *bc = malloc(sizeof (Board));
    //memcpy(bc, b, sizeof (Board));
    Move *m = malloc(sizeof (Move));
    alphabeta_negamax(&b, p, -FLT_MAX, FLT_MAX, 0, 8, m);
    int c = m->column;
    free(m);
    //free(bc);
    return c;
}
