#include <math.h>
#include "board.h"
#include "eval.h"

float eval(Board *b) {
    if (b->winner == WHITE) {
        return BETA_MAX;
    } else if (b->winner == BLACK) {
        return ALPHA_MIN;
    } else {
        return 0;
    }
}

float eval_odd_even(Board *b) {
    // TODO: implement
    return 0;
}
