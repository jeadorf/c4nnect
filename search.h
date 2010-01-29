/*! Defines all procedures related to the search algorithm itself */
#ifndef _SEARCH_H
#define	_SEARCH_H

#include "board.h"
#include "stats.h"
#include <stdint.h>

int8_t searchm(Board *b);

/*! Returns a move (i.e. a column) selected by the KI engine. Any changes to the
 * board while searching are reverted before returning. */
void search(Board *b, SearchRecord *rec);

void alphabeta_negamax(Board *b,
        float alpha, float beta,
        int8_t depth, int8_t max_depth, bool ttcuts_enabled,
        SearchRecord *rec);

#endif	/* _SEARCH_H */

