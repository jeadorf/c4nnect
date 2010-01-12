#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include "eval.h"

#define BONUS_CENTER 5

// TODO: We need some sort of randomization but generating random numbers is a
// really expensive operation that slows down the whole evaluation process.
// float value = rand() % 3 - 1.5;

float eval(Board *b) {
    float value = 0;
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
