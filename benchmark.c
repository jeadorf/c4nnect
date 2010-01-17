#include <math.h>
#include <stdint.h>
#include "benchmark.h"
#include "stats.h"
#include "board.h"
#include "search.h"
#include "eval.h"

void benchmark_run(FILE *positions_in, FILE *stats_out, FILE *summary_out) {
    fprintf(stats_out, "%-18s , %4s , %6s , %11s , %11s , %10s , %10s , %13s , %7s\n",
            "board", "move", "rating", "visited_cnt", "eval_cnt",
            "abcut_cnt", "max_depth", "reached_depth", "cpu_time");

    uint64_t n;
    SearchRecord acc;
    searchrecord_init(&acc);
    // TODO: check whether we can
    int32_t cnt = 0;
    double_t visited_cnt = 0, eval_cnt = 0, abcut_cnt = 0,
            max_depth = 0, reached_depth = 0, cpu_time = 0;
    while (fscanf(positions_in, "%lX", &n) == 1) {
        Board b;
        board_init(&b);
        board_decode(&b, n);

        SearchRecord rec;
        searchrecord_init(&rec);
        search(&b, &rec);

        fprintf(stats_out, "0x%-.16lX , %4d , %6.1f , %11ld , %11ld , %10ld , %10d , %13d , %7ld\n",
                n, rec.move, rec.rating, rec.visited_cnt, rec.eval_cnt,
                rec.abcut_cnt, rec.max_depth, rec.reached_depth, rec.cpu_time);

        visited_cnt += rec.visited_cnt;
        eval_cnt += rec.eval_cnt;
        abcut_cnt += rec.abcut_cnt;
        max_depth += rec.max_depth;
        reached_depth += rec.reached_depth;
        cpu_time += rec.cpu_time;
        cnt++;
    }

    fprintf(summary_out, "%11s , %11s , %10s , %10s , %10s , %13s , %7s\n",
            "visited_cnt", "eval_cnt", "abcut_cnt", "wincut_cnt", "max_depth", "reached_depth", "cpu_time");
    if (cnt > 0) {
        fprintf(summary_out, "%11ld , %11ld , %10ld , %10.2f , %13.2f , %7ld\n",
                (int64_t) (visited_cnt / cnt),
                (int64_t) (eval_cnt / cnt),
                (int64_t) (abcut_cnt / cnt),
                max_depth / cnt,
                reached_depth / cnt,
                (int64_t) (cpu_time / cnt));
    }
}
