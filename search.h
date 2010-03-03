/*! Defines all procedures related to the search algorithm itself */
#ifndef _SEARCH_H
#define	_SEARCH_H

#include "board.h"
#include "stats.h"
#include <stdint.h>

/*! Defines the number of milliseconds within the engine is expected to move. */
#define TIME_LIMIT_PER_PLY 1000

int8_t searchm(Board *b);

/*! Returns a move (i.e. a column) selected by the KI engine. Any changes to the
 * board while searching are reverted before returning. */
void search(Board *b, SearchRecord *rec);

/*! Orders moves. Moves that look promising come first. */
void generate_moves(SearchRecord *rec, int depth, int8_t *moves);

void alphabeta_negamax(Board *b,
        float alpha, float beta,
        int8_t depth, int8_t max_depth,
        bool ttcuts_enabled, bool defer_defeat,
        SearchRecord *rec);

#endif	/* _SEARCH_H */

