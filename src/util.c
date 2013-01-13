#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void handle_error(char *msg) {
    fputs("ERROR: ", stderr);
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(EXIT_FAILURE);
}

void print_uint64_rev(uint64_t n) {
    for (int8_t i = 0; i < 64; i++) {
        if (i != 0 && (i % 9 == 0 || i % 9 == 6)) {
            putchar('-');
        }
        putchar((n & (1UL << i)) ? '1' : '0');
    };
    printf(" --- 0x%.16" PRId64 "\n", n);
}

int64_t clock_in_millis(clock_t timespan) {
    return timespan / (CLOCKS_PER_SEC / 1000);
}
