#ifndef _EVAL_H
#define	_EVAL_H

#include "board.h"

#define ALPHA_MIN -INFINITY
#define BETA_MAX INFINITY

/*! Evaluates the position on the board from the view of player WHITE. A positive
 * return value denotes a good position for player WHITE, a negative value means
 * that BLACK has an advantage.
 */
float eval(Board *b);

/**! Checks for threats. Returned value is an estimation. */
float eval_odd_even(Board *b);

#endif	/* _EVAL_H */

