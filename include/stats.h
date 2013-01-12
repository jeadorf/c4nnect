#ifndef _STATS_H
#define _STATS_H

#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include "board.h"

/*! The variation is the result of a search. When searching we are usually
 * looking for the principal variation. */
struct Variation {
    /* Sequence of moves; upper bounded by the maximum number of plies. */
    int8_t moves[NUM_ROWS * NUM_COLS];
    /* Number of plies in this variation. */
    int8_t length;
    /*! The relative rating for this sequence of moves with respect to the
     * active player. */
    float rating;
};

typedef struct Variation Variation;

/*! Statistical record object that collects data about the search, mostly by
 * accumulation. */
struct SearchRecord {
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
    /*! Denotes whether the opponent has a winning strategy and the KI has
     * selected a variation that delays the defeat as long as possible. */
    bool defeat_deferred;
    /*! Used processor time. */
    clock_t cpu_time;
    /*! Whether the search stayed within the expected time frame set by
     * TIME_LIMIT_PER_PLY */
    bool on_time;
};

typedef struct SearchRecord SearchRecord;

void variation_init(Variation *rec);

void searchrecord_init(SearchRecord *rec);

void stats_print(Board *b, Variation *var, SearchRecord * rec);

#endif	/* _STATS_H */
