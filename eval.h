#ifndef _EVAL_H
#define _EVAL_H

#include "board.h"

/*! Initial search window. See function eval for an explanation. */
#define ALPHA_MIN -10000
#define BETA_MAX 10000

/*! Extended search window for defeat deferral search. */
#define ALPHA_MIN_DEFEAT (ALPHA_MIN - 42)
#define BETA_MAX_DEFEAT (BETA_MAX + 42)

/*! Evaluates the board position. A return value greater than zero denotes a
 * position with advantage for player WHITE, a negative value means that BLACK
 * has an advantage.
 * 
 * This function might either return an exact evaluation of the position; in
 * such a case either ALPHA_MIN or BETA_MAX is returned.  ALPHA_MIN signals
 * that the position is won for player BLACK in case of correct gameplay.
 * Conversely, BETA_MAX signals that the position is won for player WHITE.
 * 
 * Any value that is both strictly greater than ALPHA_MIN and strictly smaller
 * than BETA_MAX denotes an estimate. In this case, no certain statement can be
 * made about the position.
 */
float eval(Board *b, bool defer_defeat);

/**! Checks for threats. Returned value is an estimation. */
float eval_odd_even(Board *b);

/** Determines whether the return value of function eval is a heuristic
 * estimate of the game value or a definite statement that one of the players
 * is guaranteed to win in case of correct gameplay. */
bool winner_identified(float rating);

#endif	/* _EVAL_H */

