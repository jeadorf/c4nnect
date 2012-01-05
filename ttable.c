#include <stdbool.h>
#include <stdio.h>
#include "eval.h"
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

void ttable_read_uciml(FILE *f) {
    Board b;
    char *buf = NULL;
    size_t n = 89;
    ssize_t m = 0;
    while (!feof(f) && !ferror(f)) {
        board_init(&b);
        m = getline(&buf, &n, f);
        if (m != -1) {
            for (int8_t i = 0; i < 42; i++) {
                switch (buf[2*i]) {
                    case 'x':
                        board_put_forced(&b, WHITE, i / 6);
                        break;
                    case 'o':
                        board_put_forced(&b, BLACK, i / 6);
                        break;
                }
            }
            float r;
            switch (buf[84]) {
                case 'w':
                    r = BETA_MAX;
                    break;
                case 'd':
                    r = 0;
                    break;
                case 'l':
                    r = ALPHA_MIN;
                    break;
            }

            b.turn = WHITE;

            uint32_t hash = b.code % TTABLE_SIZE;
            TTEntry *ttentry = &(ttable[hash]);
            ttentry->code = b.code;
            ttentry->rating = r;
        }
    }
}