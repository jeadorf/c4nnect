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

/*! Configures an alpha-beta search. A configuration is not modified by the
 * alpha-beta search. */
struct SearchCfg {
    /*! The search horizon */
    int8_t max_depth;
    /*! Determines whether to use ttables for evaluating positions */
    bool ttcuts_enabled;
    /*! Whether to return shortly before exceeding the time limit*/
    bool abort_on_timeout;
    /*! Whether to look for a defeat-deferral move */
    bool defer_defeat;
};

typedef struct SearchCfg SearchCfg;

/*! Initializes a search configuration. */
void searchcfg_init(SearchCfg *cfg);

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
        int8_t depth,
        SearchCfg *cfg,
        Variation *var,
        SearchRecord *rec);

#endif	/* _SEARCH_H */

