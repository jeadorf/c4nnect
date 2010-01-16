#ifndef _SEARCH_H
#define	_SEARCH_H

#include "board.h"
#include "stats.h"

/* Gets a copy of the board and returns a move (i.e. a column) selected by the
 * KI engine. */
int search(Board *b, Player p);

void alphabeta_negamax(Board *b, Player p,
        float alpha, float beta,
        int depth, int max_depth,
        SearchRecord *rec);

#endif	/* _SEARCH_H */

