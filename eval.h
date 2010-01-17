#ifndef _EVAL_H
#define	_EVAL_H

#include <math.h>
#include "board.h"

#define ALPHA_MIN -INFINITY
#define BETA_MAX INFINITY

/*
 * Evaluates the position on the board from the view of player WHITE. A positive
 * return value denotes a good position for player WHITE, a negative value means
 * that BLACK has an advantage.
 */
float eval(Board *b);

#endif	/* _EVAL_H */

