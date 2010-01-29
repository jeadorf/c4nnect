#ifndef _BENCHMARK_H
#define	_BENCHMARK_H

#include <stdio.h>

/*! Examines a number of positions and prints the results in a table. The table
 * data maps input data (board position) to search outcomes and statistics about
 * time, number of positions, search depth, cpu time, and so on.
 */
void benchmark_run(FILE *boards_in, FILE *stats_out, FILE *summary_out);

/*! Generate a bunch of sample positions. */
void benchmark_sample(FILE *boards_out);

#endif	/* _BENCHMARK_H */

