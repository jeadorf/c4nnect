#ifndef _STATS_H
#define	_STATS_H

#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>

struct Statistics {
    /* Number of evaluated positions during search */
    unsigned long eval_cnt;
    /* Number of visited positions during search. This includes both leaves and
     * inner nodes of the search tree.*/
    unsigned long visited_cnt;
    /* Number of times a subtree has been pruned by alpha beta */
    unsigned long abcut_cnt;
    /* Number of times a subtree has been pruned because a winning move has been
     * found */
    unsigned long wincut_cnt;
    /* The search depth. */
    int max_depth;
    int reached_depth;
    /* Denotes whether the search has lead to a winning strategy for one of
     * the players. */
    bool forced_variation;
    /* Used processor time. */
    clock_t cpu_time;
};

typedef struct Statistics Statistics;

void stats_init(Statistics *stats);

#endif	/* _STATS_H */
