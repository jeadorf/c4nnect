#include <math.h>

#include "benchmark.h"
#include "stats.h"
#include "board.h"
#include "search.h"
#include "eval.h"

void benchmark_generate(FILE *fdin, FILE *fdout) {
    // Print table header
    fprintf(fdout, "%-16s , %-16s , %4s , %6s , %11s , %11s , %10s , %10s , %10s , %10s , %7s\n",
            "white", "black", "move", "rating", "visited_cnt", "eval_cnt",
            "abcut_cnt", "wincut_cnt", "max_depth", "reached_depth", "cpu_time");

    // TODO: complete benchmarking system
    Board b;
    board_init(&b);
    Player p = WHITE;
    int winmv = -1;
    unsigned long prim_hash, snd_hash, hash;
    unsigned long acc_cpu_time = 0UL;
    while (!board_full(&b) && b.winner == NOBODY) {
        SearchRecord rec;
        searchrecord_init(&rec);
        search(&b, p, &rec);
        float rating = (rec.rating >= BONUS_WIN ? INFINITY : (rec.rating <= -BONUS_WIN ? -INFINITY : rec.rating));
        // FIXME: board hex-value representation
        fprintf(fdout, "%-16lX , %-16lX , %4d , %6.1f , %11ld , %11ld , %10ld , %10ld , %10d , %10d , %7ld\n",
                prim_hash, snd_hash, rec.move, rating, rec.visited_cnt, rec.eval_cnt,
                rec.abcut_cnt, rec.wincut_cnt, rec.max_depth, rec.reached_depth, rec.cpu_time);
        board_hash(&b, &prim_hash, &snd_hash, &hash);
        acc_cpu_time += rec.cpu_time;
        if (rec.winner_identified && winmv == -1) {
            winmv = b.move_cnt;
        }
        board_put(&b, p, rec.move);
        p = other(p);
        board_print(&b);
    }
    printf("Winner identified in ply %d\n", winmv);
    printf("cpu_time: %d\n", (1000 * acc_cpu_time / CLOCKS_PER_SEC));
}
