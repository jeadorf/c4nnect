#include <stdbool.h>
#include "stats.h"

void stats_init(Statistics *stats) {
    stats->eval_cnt = 0UL;
    stats->visited_cnt = 0UL;
    stats->abcut_cnt = 0UL;
    stats->wincut_cnt = 0UL;
    stats->max_depth = 0;
    stats->reached_depth = 0;
    stats->winner_identified = false;
    stats->defeat_deferred = false;
    stats->cpu_time = 0UL;
}
