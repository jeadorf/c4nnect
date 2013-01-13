#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "benchmark.h"
#include "board.h"
#include "eval.h"
#include "search.h"
#include "stats.h"
#include "ttable.h"

void benchmark_run(FILE *positions_in, FILE *stats_out, FILE *summary_out) {
    if (stats_out != NULL) {
        fprintf(stats_out,
                "%-18s , %8s, %4s , %8s , %11s , %11s , "
                "%10s , %10s , %11s , %9s , %8s , "
                "%10s , %13s , %7s , %7s\n",
                "board", "move_cnt", "move", "rating", "visited_cnt", "eval_cnt",
                "abcut_cnt", "ttcut_cnt", "ttadd_cnt", "ttrcoll_cnt", "ttcharge",
                "max_depth", "reached_depth", "cpu_time", "on_time");
    }
    uint64_t n;
    SearchRecord acc;
    searchrecord_init(&acc);
    int32_t cnt = 0;
    uint64_t visited_cnt = 0, move_cnt = 0, eval_cnt = 0,
            abcut_cnt = 0, ttcut_cnt = 0, ttrcoll_cnt = 0, ttadd_cnt = 0,
            max_depth = 0, reached_depth = 0, cpu_time = 0, on_time_cnt = 0,
            solved = 0;
    while (fscanf(positions_in, "%lX", &n) == 1) {
        Board b;
        board_decode(&b, n); // FIXME: probably something to do with PRId32 etc
        Variation var;
        variation_init(&var);
        SearchRecord rec;
        searchrecord_init(&rec);
        search(&b, &var, &rec);

        int cpu_time_ms = (int) (rec.cpu_time / (CLOCKS_PER_SEC / 1000));
        
        if (stats_out != NULL) {
            fprintf(stats_out,
                    "0x%-.16lX , %8d, %4d , %8.1f , %11ld , "
                    "%11ld , %10ld , %10ld , %11ld , %9ld , %8.2f, "
                    "%10d , %13d , %7" PRId32 " , %7d\n",
                    n, b.move_cnt, var.moves[0], var.rating, rec.visited_cnt, rec.eval_cnt,
                    rec.abcut_cnt, rec.ttcut_cnt, rec.ttadd_cnt, rec.ttrcoll_cnt, rec.ttcharge,
                    rec.max_depth, rec.reached_depth, cpu_time_ms, rec.on_time ? 1 : 0);
        }
        move_cnt += b.move_cnt;
        visited_cnt += rec.visited_cnt;
        eval_cnt += rec.eval_cnt;
        abcut_cnt += rec.abcut_cnt;
        ttcut_cnt += rec.ttcut_cnt;
        ttadd_cnt += rec.ttadd_cnt;
        ttrcoll_cnt += rec.ttrcoll_cnt;
        max_depth += rec.max_depth;
        reached_depth += rec.reached_depth;
        cpu_time += rec.cpu_time;
        on_time_cnt += (rec.on_time ? 1 : 0);
        solved += (winner_identified(var.rating) ? 1 : 0);
        cnt++;
    }

    if (summary_out != NULL) {
        fprintf(summary_out, "%7s , %8s , %11s , %11s , "
                "%10s , %10s , %10s , %10s , %15s , "
                "%10s , %13s , %7s , %7s\n",
                "solved", "move_cnt", "visited_cnt", "eval_cnt",
                "abcut_cnt", "ttcut_cnt", "ttadd_cnt", "ttrcoll_cnt", "final ttcharge",
                "max_depth", "reached_depth", "cpu_time", "on_time");
        if (cnt > 0) {
            double dcnt = (double) cnt;
            fprintf(summary_out, "%6.3f%% , %8d , %11ld , %11ld , %10ld , %10ld , %10ld , %10ld , "
                    "%15.2f ,  %10.2f , %13.2f , %7ld , %7.2f\n",
                    (solved / dcnt),
                    (int8_t) (move_cnt / dcnt),
                    (int64_t) (visited_cnt / dcnt),
                    (int64_t) (eval_cnt / dcnt),
                    (int64_t) (abcut_cnt / dcnt),
                    (int64_t) (ttcut_cnt / dcnt),
                    (int64_t) (ttadd_cnt / dcnt),
                    (int64_t) (ttrcoll_cnt / dcnt),
                    ttable_charge(),
                    max_depth / dcnt,
                    reached_depth / dcnt,
                    (int64_t) (cpu_time / dcnt / (CLOCKS_PER_SEC / 1000)),
                    on_time_cnt / dcnt);
        }
    }
}

void benchmark_sample(FILE *boards_out) {
    srand(time(0));
    // Idea: generate random move sequence with a fixed number of moves. It is
    // wasted if it leads to a win for one of the players, otherwise written
    // to the list of positions.
    int32_t remaining_samples = 250;
    while (remaining_samples > 0) {
        int8_t npieces = rand() % (int) (NUM_ROWS * NUM_COLS);
        bool failed = true;
        Board b;

        do {
            failed = false;
            board_init(&b);
            putchar('.');
            for (int i = 0; i < npieces; i++) {
                int8_t col = rand() % NUM_COLS;
                while (board_column_full(&b, col)) {
                    col += 3;
                    col %= NUM_COLS;
                }
                board_put(&b, col);
                if (b.winner != NOBODY) {
                    failed = true;
                    break;
                }
            }
        } while (failed);

        fprintf(boards_out, "0x%.16lX\n", b.code);
        fflush(boards_out);
        board_print(&b);

        remaining_samples--;
    }
}
