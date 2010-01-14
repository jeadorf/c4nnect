#include "stats.h"

void stats_init(Statistics *stats) {
    stats->eval_cnt = 0;
    stats->visited_cnt = 0;
    stats->abcut_cnt = 0;
    stats->wincut_cnt = 0;
    stats->max_depth = 0;
    stats->reached_depth = 0;
    stats->forced_variation = 0;
    stats->cpu_time = 0;
}
