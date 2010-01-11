#include <float.h>
#include <stdio.h>
#include "eval.h"

#define BONUS_WIN 10e32
#define BONUS_CENTER 25

float eval(Board *b) {
    float value = 0;

    // Fast evaluation
    if (b->winner == WHITE) {
        return BONUS_WIN;
    } else if (b->winner == BLACK) {
        return -BONUS_WIN;
    }
    
    if (board_get_top(b, 2) == WHITE) {
        value += BONUS_CENTER;
    } else if (board_get_top(b, 2) == BLACK) {
        value -= BONUS_CENTER;
    } else if (board_get_top(b, 3) == WHITE) {
        value += BONUS_CENTER;
    } else if (board_get_top(b, 3) == BLACK) {
        value -= BONUS_CENTER;
    } else if (board_get_top(b, 4) == WHITE) {
        value += BONUS_CENTER;
    } else if (board_get_top(b, 4) == BLACK) {
        value -= BONUS_CENTER;
    }

    return value;
}
