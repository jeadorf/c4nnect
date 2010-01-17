#ifndef _STATS_H
#define	_STATS_H

#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>

struct SearchRecord {
    /* Recommended move */
    uint8_t move;
    /* Rating of the move */
    float rating;
    /* Number of evaluated positions during search */
    uint64_t eval_cnt;
    /* Number of visited positions during search. This includes both leaves and
     * inner nodes of the search tree.*/
    uint64_t visited_cnt;
    /* Number of times a subtree has been pruned by alpha beta */
    uint64_t abcut_cnt;
    /* Number of times a subtree has been pruned because a winning move has been
     * found */
    uint64_t wincut_cnt;
    /* The search depth. */
    uint8_t max_depth;
    uint8_t reached_depth;
    /* Denotes whether the search has lead to a winning strategy for one of
     * the players. */
    bool winner_identified;
    /* Denotes whether the computer has recognized that he will lose to an
     * opponent that plays optimally and that he now tries to defer the defeat
     * as long as possible. */
    bool defeat_deferred;
    /* Used processor time. */
    clock_t cpu_time;
};

typedef struct SearchRecord SearchRecord;

void searchrecord_init(SearchRecord *rec);

#endif	/* _STATS_H */
