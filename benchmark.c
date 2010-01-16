#include "benchmark.h"
#include "stats.h"

void benchmark_generate(FILE *fdin, FILE *fdout) {
    // Print table header
    fprintf(fdout, "%-10s , %-10s , %4s , %5s , %11s , %11s , %10s , %10s , %2s , %2s , %7s\n",
            "white", "black", "move", "value", "visited_cnt", "eval_cnt",
            "abcut_cnt", "wincut_cnt", "max_depth", "reached_depth", "cpu_time");
}
