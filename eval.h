#ifndef _EVAL_H
#define	_EVAL_H

#include <float.h>
#include "board.h"

/* Ultimate evaluation result that denotes a win. */
#define BONUS_WIN 10e8

/*
 * Evaluates the position on the board from the view of player WHITE. A positive
 * return value denotes a good position for player WHITE, a negative value means
 * that BLACK has an advantage.
 */
float eval(Board *b);

#endif	/* _EVAL_H */

