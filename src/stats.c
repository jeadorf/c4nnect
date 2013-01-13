#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "eval.h"
#include "search.h"
#include "stats.h"
#include "ttable.h"

void variation_init(Variation *var) {
    var->length = 0;
    var->rating = 0;
}

void searchrecord_init(SearchRecord *rec) {
    rec->eval_cnt = 0UL;
    rec->visited_cnt = 0UL;
    rec->abcut_cnt = 0UL;
    rec->ttcut_cnt = 0UL;
    rec->ttadd_cnt = 0UL;
    rec->ttrcoll_cnt = 0UL;
    rec->ttcharge = 0UL;
    rec->max_depth = 0;
    rec->reached_depth = 0;
    rec->defeat_deferred = false;
    rec->cpu_time = 0UL;
    rec->on_time = true;
}

void stats_print(Board *b, Variation *var, SearchRecord * rec) {
    // Print statistics
    board_print(b);
    printf("%19s : %s\n", "Turn", name(b->turn));
    printf("%19s : %d\n", "Move", var->moves[0]);
    printf("%19s : ", "Principal variation");
    if (rec->defeat_deferred) {
        printf("defeat deferred");
    } else {
        for (int i = 0; i < var->length; i++) {
            printf("%d ", var->moves[i]);
        }
    }
    putchar('\n');
    if (var->rating >= BETA_MAX) {
        printf("%19s : %s will win\n", "Absolute rating", name(b->turn));
    } else if (var->rating <= ALPHA_MIN) {
        printf("%19s : %s will win\n", "Absolute rating", name(other(b->turn)));
    } else {
        printf("%19s : %.1f\n", "Absolute rating", b->turn == WHITE ? var->rating : -var->rating);
    }
    printf("%19s : %.1f\n", "Relative rating", var->rating);
    printf("%19s : %d\n", "Winner identified", winner_identified(var->rating));
    printf("%19s : %d\n", "Defeat deferred", rec->defeat_deferred);
    printf("%19s : %d\n", "Maximum depth", rec->max_depth);
    printf("%19s : %d\n", "Reached depth", rec->reached_depth);
    printf("%19s : %" PRId64 "\n", "Evaluations", rec->eval_cnt);
    printf("%19s : %" PRId64 "\n", "Positions", rec->visited_cnt);
    // The average branching does not give a feedback on good move ordering
    // since it is not weighted and the inner nodes near the leaves dominate
    // this calculation
    printf("%19s : %.2f\n", "Average branching", (rec->visited_cnt - 1.0) / (rec->visited_cnt - rec->eval_cnt));
    printf("%19s : %" PRId64 "\n", "Alpha-Beta cuts", rec->abcut_cnt);
    printf("%19s : %" PRId64 "\n", "TT cuts", rec->ttcut_cnt);
    printf("%19s : %" PRId64 "\n", "TT inserts", rec->ttadd_cnt);
    printf("%19s : %.1f %%\n", "TT inserts / pos", (100.0 * rec->ttadd_cnt) / rec->visited_cnt);
    printf("%19s : %" PRId64 "\n", "TT read collisions", rec->ttrcoll_cnt);
    printf("%19s : %.1f %%\n", "TT charge", rec->ttcharge * 100);
    printf("%19s : %d\n", "TT entries", ttable_entry_cnt());
    printf("%19s : %d\n", "TT size", TTABLE_SIZE);
    printf("%19s : %d ms\n", "CPU time", (int) (rec->cpu_time / (CLOCKS_PER_SEC / 1000)));
    printf("%19s : %3s\n", "Returned on time", rec->on_time ? "yes" : "no");
    printf("%19s : 0x%.16lX\n", "Board", b->code);
    putchar('\n');
}
