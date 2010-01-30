#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include "eval.h"
#include "ttable.h"
#include "stats.h"

void searchrecord_init(SearchRecord *rec) {
    rec->move = 0;
    rec->pv.length = 0;
    rec->rating = 0;
    rec->eval_cnt = 0UL;
    rec->visited_cnt = 0UL;
    rec->abcut_cnt = 0UL;
    rec->ttcut_cnt = 0UL;
    rec->ttadd_cnt = 0UL;
    rec->ttrcoll_cnt = 0UL;
    rec->ttcharge = 0UL;
    rec->max_depth = 0;
    rec->reached_depth = 0;
    rec->winner_identified = false;
    rec->defeat_deferred = false;
    rec->cpu_time = 0UL;
}

void stats_print(Board *b, SearchRecord * rec) {
    // Print statistics
    board_print(b);
    printf("%19s : %s\n", "Turn", b->turn == WHITE ? "White" : "Black");
    printf("%19s : %d\n", "Move", rec->move);
    printf("%19s : ", "Principal variation");
    for (int i = 0; i < rec->pv.length; i++) {
        printf("%d ", rec->pv.moves[i]);
    }
    putchar('\n');
    if (rec->rating >= BETA_MAX) {
        printf("%19s : %s\n", "Absolute rating", b->turn == WHITE ? "White will win" : "Black will win");
    } else if (rec->rating <= ALPHA_MIN) {
        printf("%19s : %s\n", "Absolute rating", other(b->turn) == WHITE ? "White will win" : "Black will win");
    } else {
        printf("%19s : %.1f\n", "Absolute rating", b->turn == WHITE ? rec->rating : -rec->rating);
    }
    printf("%19s : %.1f\n", "Relative rating", rec->rating);
    printf("%19s : %d\n", "Winner identified", rec->winner_identified);
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
    printf("%19s : 0x%.16" PRIu64 "\n", "Board", b->code);
    putchar('\n');
}