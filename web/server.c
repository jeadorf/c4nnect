#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "benchmark.h"
#include "board.h"
#include "board2eps.h"
#include <inttypes.h>

#include "search.h"
#include "stats.h"
#include "ucitable.h"
#include "parser.h"
#include "eval.h"
#include "ttable.h"

static void print_board(Board *b) {
    puts("{");
    printf("  \"code\": \"%" PRIu64 "\",\n", b->code);
    printf("  \"winner\": %d,\n", (b->winner + 1) % 3);
    puts("  \"pieces\": [");
    for (int c = 0; c < NUM_COLS; c++) {
        if (c) {
            puts(",");
        }
        fputs("    [", stdout);
        for (int r = 0; r < NUM_ROWS; r++) {
             if (r) {
               fputs(",", stdout);
             }
             Player p = board_get(b, r, c);
             if (p == WHITE) {
                 printf("%d", 1);
             } else if (p == BLACK) {
                 printf("%d", 2);
             } else {
                 printf("%d", 0);
             }
        }
        fputs("]", stdout);
    }
    puts("]");
    fputs("}", stdout);
}

static void print_next_positions(Board *b) {
    puts("[");
    for (int c = 0; c < NUM_COLS; c++) {
        if (c) {
           puts(",");
        }
        if (board_column_full(b, c) || (b->winner != NOBODY)) {
            puts("0");
        } else {
            board_put(b, c);
            print_board(b);
            board_undo(b, c);
        }
    }
    fputs("]", stdout);
}

static void print_response(Board *b) {
    puts("{");
    fputs("\"current\": ", stdout);
    print_board(b);
    puts(",");
    fputs("\"next\": ", stdout);
    print_next_positions(b);
    puts("}");
}

int main(int argc, char** argv) {
    if (argc != 3) {
        return (EXIT_FAILURE);
    }

    Board b;
    board_init(&b);
    uint64_t code = strtoull(argv[2], NULL, 10);
    board_decode(&b, code);
    if (!strcmp(argv[1], "get")) {
        print_response(&b);
        return (EXIT_SUCCESS);
    } else if (!strcmp(argv[1], "move")) {
        if (b.winner == NOBODY && !board_full(&b)) {
            Variation var;
            variation_init(&var);
            SearchRecord rec;
            searchrecord_init(&rec);
            search(&b, &var, &rec);
            board_put(&b, var.moves[0]);
        }
        print_response(&b);
        return (EXIT_SUCCESS);
    } else {
        return (EXIT_FAILURE);
    }
}
