#include <stdlib.h>

#include "board.h"
#include "eval.h"
#include "ucitable.h"

UCITEntry ucitable[UCITABLE_SIZE];

UCITEntry* ucitable_lookup(uint64_t code) {
    uint32_t h = code % UCITABLE_SIZE;
    UCITEntry *e = &(ucitable[h]);
    while (e != NULL && e->code != code) {
        e = e->next;
    }
    return e;
}

void ucitable_read(FILE *f) {
    Board b;
    char *buf = NULL;
    size_t n = 89;
    ssize_t m = 0;
    int c = 0;
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
            float r = 0;
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

            uint32_t hash = b.code % UCITABLE_SIZE;
            UCITEntry *ucitentry = &(ucitable[hash]);
            if (ucitentry->code != 0) {
                UCITEntry *elem;
                elem = (UCITEntry*) malloc(sizeof(UCITEntry));
                elem->next = NULL;
                ucitentry->next = elem;
                ucitentry = elem;
            }
            ucitentry->code = b.code;
            ucitentry->rating = r;
        }
    }
}
