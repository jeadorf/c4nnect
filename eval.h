#ifndef _EVAL_H
#define _EVAL_H

#include "board.h"
        
/*! Define the bounds of the search window */
#define ALPHA_MIN -10000
#define BETA_MAX 10000

/*! Extended search window for defeat deferral search. */
#define ALPHA_MIN_DEFEAT (ALPHA_MIN - 42)
#define BETA_MAX_DEFEAT (BETA_MAX + 42)

/*! Evaluates the position on the board from the view of player WHITE. A positive
 * return value denotes a good position for player WHITE, a negative value means
 * that BLACK has an advantage.
 */
float eval(Board *b, bool defer_defeat);

/**! Checks for threats. Returned value is an estimation. */
float eval_odd_even(Board *b);

bool winner_identified(float rating);

#endif	/* _EVAL_H */

