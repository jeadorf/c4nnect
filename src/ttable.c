#include <stdbool.h>
#include <stdio.h>

#include "ttable.h"

TTEntry ttable[TTABLE_SIZE];

float ttable_charge() {
    return ((float) ttable_entry_cnt()) / TTABLE_SIZE;
}

int32_t ttable_entry_cnt() {
    int32_t fill_cnt = 0;
    for (int i = 0; i < TTABLE_SIZE; i++) {
        if (ttable[i].code != 0) {
            fill_cnt++;
        }
    }
    return fill_cnt;
}

void ttable_write(FILE *f) {
    int *d = (int*) ttable;
    for (uint32_t i = 0; i < TTABLE_SIZE * sizeof(TTEntry) / sizeof(int); i++) {
        putc(*d, f);
        d++;
    }
}

void ttable_read(FILE *f) {
    int *d = (int*) ttable;
    for (uint32_t i = 0; i < TTABLE_SIZE * sizeof(TTEntry) / sizeof(int); i++) {
        *d = getc(f);
        d++;
    }
}
