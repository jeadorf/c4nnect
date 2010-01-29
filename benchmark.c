#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "benchmark.h"
#include "stats.h"
#include "board.h"
#include "search.h"
#include "ttable.h"

void benchmark_run(FILE *positions_in, FILE *stats_out, FILE *summary_out) {
    fprintf(stats_out,
            "%-18s , %4s , %6s , %11s , %11s , "
            "%10s , %10s , %10s , %10s , %9s , "
            "%10s , %13s , %7s\n",
            "board", "move", "rating", "visited_cnt", "eval_cnt",
            "abcut_cnt", "ttcut_cnt", "ttadd_cnt", "ttrcoll_cnt", "ttcharge",
            "max_depth", "reached_depth", "cpu_time");

    uint64_t n;
    SearchRecord acc;
    searchrecord_init(&acc);
    int32_t cnt = 0;
    double_t visited_cnt = 0, eval_cnt = 0,
            abcut_cnt = 0, ttcut_cnt = 0, ttrcoll_cnt = 0, ttadd_cnt = 0,
            max_depth = 0, reached_depth = 0, cpu_time = 0, solved = 0;
    // FIXME: %llX in Debug-32, %lX in Debug-64
    while (fscanf(positions_in, "%lX", &n) == 1) {
        Board b;
        board_init(&b);
        board_decode(&b, n);
        
        SearchRecord rec;
        searchrecord_init(&rec);
        search(&b, &rec);

        fprintf(stats_out,
                "0x%-.16lX , %4d , %6.1f , %11ld , "
                "%11ld , %10ld , %10ld , %10ld , %10ld , %9.2f, "
                "%10d , %13d , %7ld\n",
                n, rec.move, rec.rating, rec.visited_cnt, rec.eval_cnt,
                rec.abcut_cnt, rec.ttcut_cnt, rec.ttadd_cnt, rec.ttrcoll_cnt, rec.ttcharge,
                rec.max_depth, rec.reached_depth, rec.cpu_time / (CLOCKS_PER_SEC / 1000));

        visited_cnt += rec.visited_cnt;
        eval_cnt += rec.eval_cnt;
        abcut_cnt += rec.abcut_cnt;
        ttcut_cnt += rec.ttcut_cnt;
        ttadd_cnt += rec.ttadd_cnt;
        ttrcoll_cnt += rec.ttrcoll_cnt;
        max_depth += rec.max_depth;
        reached_depth += rec.reached_depth;
        cpu_time += rec.cpu_time;
        solved += rec.winner_identified;
        cnt++;
    }

    fprintf(summary_out, "%7s , %11s , %11s , "
            "%10s , %10s , %10s , %10s , %15s , "
            "%10s , %13s , %7s\n",
            "solved", "visited_cnt", "eval_cnt",
            "abcut_cnt", "ttcut_cnt","ttadd_cnt", "ttrcoll_cnt", "final ttcharge",
            "max_depth", "reached_depth", "cpu_time");
    if (cnt > 0) {
        fprintf(summary_out, "%6.3f%% , %11ld , %11ld , %10ld , %10ld , %10ld , %10ld , "
                "%15.2f ,  %10.2f , %13.2f , %7ld\n",
                (solved / cnt),
                (int64_t) (visited_cnt / cnt),
                (int64_t) (eval_cnt / cnt),
                (int64_t) (abcut_cnt / cnt),
                (int64_t) (ttcut_cnt / cnt),
                (int64_t) (ttadd_cnt / cnt),
                (int64_t) (ttrcoll_cnt / cnt),
                ttable_charge(),
                max_depth / cnt,
                reached_depth / cnt,
                (int64_t) (cpu_time / cnt / (CLOCKS_PER_SEC / 1000)));
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
                int8_t col = rand() % 6;
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
        printf("\n0x%.16lX\n", b.code);
        board_print(&b);

        remaining_samples--;
    }
}
