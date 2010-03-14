/*! Defines all procedures related to the search algorithm itself */
#ifndef _SEARCH_H
#define	_SEARCH_H

#include "board.h"
#include "stats.h"
#include <stdint.h>

/*! Defines the number of milliseconds within the engine is expected to move. */
#define TIME_LIMIT_PER_PLY 1000
/*! Defines the number of milliseconds that form a safety margin. After the
 * abortion of the last iteration, the search will still need some time. The
 * safety margin makes sure that the search will abort even before reaching
 * the time limit, thus preserving a little time for finishing. */
#define TIME_LIMIT_SAFETY_MARGIN 30

/*! Returns a move (i.e. a column) selected by the KI engine. Any changes to the
 * board while searching are reverted before returning. */
int8_t searchm(Board *b);

void search(Board *b, Variation *var, SearchRecord *rec);

/*! Orders moves. Moves that look promising come first. */
void generate_moves(Variation *var, int depth, int8_t *moves);

// TODO: introduce either named parameter values for bool parameters or introduce
//       configuration data structure
bool alphabeta_negamax(Board *b,
        float alpha, float beta,
        int8_t depth, int8_t max_depth,
        bool abort_on_timeout, bool ttcuts_enabled, bool defer_defeat,
        Variation *var,
        SearchRecord *rec);

#endif	/* _SEARCH_H */

