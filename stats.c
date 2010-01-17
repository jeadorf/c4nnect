#include <stdbool.h>
#include "stats.h"

void searchrecord_init(SearchRecord *rec) {
    rec->move = 0;
    rec->rating = 0;
    rec->eval_cnt = 0UL;
    rec->visited_cnt = 0UL;
    rec->abcut_cnt = 0UL;
    rec->ttcut_cnt = 0UL;
    rec->ttrcoll_cnt = 0UL;
    rec->max_depth = 0;
    rec->reached_depth = 0;
    rec->winner_identified = false;
    rec->defeat_deferred = false;
    rec->cpu_time = 0UL;
}
