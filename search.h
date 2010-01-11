#ifndef _SEARCH_H
#define	_SEARCH_H

#include "board.h"

/* Gets a copy of the board and returns a move (i.e. a column) selected by the
 * KI engine.
 */
int search(Board b, Player p);

#endif	/* _SEARCH_H */

