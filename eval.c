#include <math.h>
#include "board.h"
#include "eval.h"

static int count_threats(Board *b) {
    int8_t threats = 0;
    for (int8_t col = 0; col < NUM_COLS; col++) {
        int8_t top =  b->tops[col];
        for (int8_t row = top; row < NUM_ROWS; row++) {
            if (board_is_threat(b, WHITE, row, col)) {
                threats++;
            }
            if (board_is_threat(b, BLACK, row, col)) {
                threats--;
            }
        }
    }
    return threats;
}

float eval(Board *b) {
    if (b->winner == WHITE) {
        return BETA_MAX;
    } else if (b->winner == BLACK) {
        return ALPHA_MIN;
    } else {
        return count_threats(b);
    }
}

float eval_odd_even(Board *b) {
    // TODO: implement
    return 0;
}
