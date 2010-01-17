#ifndef _SEARCH_H
#define	_SEARCH_H

#include "board.h"
#include "stats.h"
#include <stdint.h>

/* Forwards to 'search' but just returns the move and does not expose the
 * full search record. */
int8_t searchm(Board *b, Player p);

/* Returns a move (i.e. a column) selected by the KI engine. Any changes to the
 * board while searching are reverted before returning. */
void search(Board *b, Player p, SearchRecord *rec);

void alphabeta_negamax(Board *b, Player p,
        float alpha, float beta,
        int8_t depth, int8_t max_depth,
        SearchRecord *rec);

#endif	/* _SEARCH_H */

