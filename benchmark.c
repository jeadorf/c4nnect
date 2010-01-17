#include <math.h>

#include "benchmark.h"
#include "stats.h"
#include "board.h"
#include "search.h"
#include "eval.h"

void benchmark_run(FILE *fdin, FILE *fdout) {
    // Print table header
    fprintf(fdout, "%-18s , %4s , %6s , %11s , %11s , %10s , %10s , %10s , %10s , %7s\n",
            "board", "move", "rating", "visited_cnt", "eval_cnt",
            "abcut_cnt", "wincut_cnt", "max_depth", "reached_depth", "cpu_time");
    uint64_t n;
    while (fscanf(fdin, "%lX", &n) == 1) {
        Board b;
        board_init(&b);
        board_decode(&b, n);

        SearchRecord rec;
        searchrecord_init(&rec);
        // FIXME: encode the player that is to move into board code
        search(&b, WHITE, &rec);
        // TODO: fix rating infinity / big value problem
        float rating = (rec.rating >= BONUS_WIN ? INFINITY : (rec.rating <= -BONUS_WIN ? -INFINITY : rec.rating));
        fprintf(fdout, "0x%-.16lX , %4d , %6.1f , %11ld , %11ld , %10ld , %10ld , %10d , %10d , %7ld\n",
                n, rec.move, rating, rec.visited_cnt, rec.eval_cnt,
                rec.abcut_cnt, rec.wincut_cnt, rec.max_depth, rec.reached_depth, rec.cpu_time);
    }
    // TODO: printf summary in DEBUG mode
}
