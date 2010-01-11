#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include "eval.h"

#define BONUS_WIN 100
#define BONUS_CENTER 5

float eval(Board *b) {
    float value = rand() % 10 - 5;

    if (b->winner == WHITE) {
        value += BONUS_WIN;
    } else if (b->winner == BLACK) {
        value -= BONUS_WIN;
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
