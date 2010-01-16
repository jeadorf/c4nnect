#ifndef _SEARCH_H
#define	_SEARCH_H

#include "board.h"
#include "stats.h"

/* Forwards to 'search' but just returns the move and does not expose the
 * full search record. */
int searchm(Board *b, Player p);

/* Returns a move (i.e. a column) selected by the KI engine. Any changes to the
 * board while searching are reverted before returning. */
void search(Board *b, Player p, SearchRecord *rec);

void alphabeta_negamax(Board *b, Player p,
        float alpha, float beta,
        int depth, int max_depth,
        SearchRecord *rec);

#endif	/* _SEARCH_H */

