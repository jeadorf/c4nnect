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

float eval(Board *b, bool defer_defeat) {
    if (b->winner == WHITE) {
        // When doing a normal search, win positions all have the same value.
        // When we are trying to defer a defeat win positions that are very
        // close are more valuable. This makes the AI avoid to lose very early.
        return BETA_MAX + (defer_defeat ? 42 - b->move_cnt : 0);
    } else if (b->winner == BLACK) {
        return ALPHA_MIN - (defer_defeat ? 42 - b->move_cnt : 0);
    } else {
        return 0;
    }
}

float eval_odd_even(Board *b) {
    // TODO: implement
    return 0;
}
