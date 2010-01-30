#ifndef _STATS_H
#define	_STATS_H

#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include "board.h"

struct Variation {
    /* Sequence of moves */
    int8_t moves[NUM_ROWS * NUM_COLS];
    /* Number of plies */
    int8_t length;
};

typedef struct Variation Variation;

struct SearchRecord {
    /*! Principal variation. */
    Variation pv;
    /*! Rating of the principal variation. */
    float rating;
    /*! Number of evaluated positions during search. */
    int64_t eval_cnt;
    /*! Number of visited positions during search. This includes both leaves and
     * inner nodes of the search tree.*/
    int64_t visited_cnt;
    /*! Number of times a subtree has been pruned by alpha beta. */
    int64_t abcut_cnt;
    /*! The maximum search depth. */
    int8_t max_depth;
    /*! The maximum reached search depth. */
    int8_t reached_depth;
    /*! The number of successful transposition table lookups. */
    int64_t ttcut_cnt;
    /*! The number of times an entry has been added to the transposition table. */
    int64_t ttadd_cnt;
    /*! The number of read collisions transposition table lookups. This counts the
     * number of times an entry was read in the transposition table but belonged
     * to another position than the one currently evaluated. */
    int64_t ttrcoll_cnt;
    /*! The percentage to which the transposition table is filled with positions. */
    float ttcharge;
    /*! Denotes whether the search has lead to a winning strategy for one of
     * the players. */
    bool winner_identified;
    /*! Denotes whether the opponent has a winning strategy and the KI has
     * selected a variation that delays the defeat as long as possible. */
    bool defeat_deferred;
    /*! Used processor time. */
    clock_t cpu_time; // TODO: naming clock vs. milliseconds
};

typedef struct SearchRecord SearchRecord;

void searchrecord_init(SearchRecord *rec);

void stats_print(Board *b, SearchRecord * rec);

#endif	/* _STATS_H */
