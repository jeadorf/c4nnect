#ifndef _EVAL_H
#define	_EVAL_H

#include "board.h"

/*
 * Evaluates the position on the board from the view of player WHITE. A positive
 * return value denotes a good position for player WHITE, a negative value means
 * that BLACK has an advantage.
 */
float eval(Board *b);

#endif	/* _EVAL_H */

