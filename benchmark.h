#ifndef _BENCHMARK_H
#define	_BENCHMARK_H

#include <stdio.h>

/* Examines a number of positions and prints the results in a table. The table
 * data maps input data (board position) to search outcomes and statistics about
 * time, number of positions, search depth, cpu time, and so on.
 */
void benchmark_generate(FILE *fdin, FILE *fdout);

#endif	/* _BENCHMARK_H */
