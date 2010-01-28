#include <math.h>
#include "eval.h"

// TODO: We need some sort of randomization but generating random numbers is a
// really expensive operation that slows down the whole evaluation process.
// float value = rand() % 3 - 1.5;

float eval(Board *b) {
    if (b->winner == WHITE) {
        return BETA_MAX;
    } else if (b->winner == BLACK) {
        return ALPHA_MIN;
    } else {
        return 0;
    }
}
