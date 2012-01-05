#ifndef _UTIL_H
#define _UTIL_H

#include <stdint.h>
#include <time.h>

void handle_error(char *msg);

void print_uint64_rev(uint64_t n);

int64_t clock_in_millis(clock_t timespan);

#endif	/* _UTIL_H */
