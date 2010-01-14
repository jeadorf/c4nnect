#ifndef _SEARCH_H
#define	_SEARCH_H

#include "board.h"
#include "stats.h"

/* Gets a copy of the board and returns a move (i.e. a column) selected by the
 * KI engine. */
int search(Board *b, Player p);

// TODO: maybe make move initialization explicit
struct Move {
    int column;
    float value;
};

typedef struct Move Move;

void alphabeta_negamax(Board *b, Player p,
                       float alpha, float beta,
                       int depth, int max_depth,
                       Move *result, Statistics *stats);

#endif	/* _SEARCH_H */

