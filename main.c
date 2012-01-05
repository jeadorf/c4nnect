#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "benchmark.h"
#include "board.h"
#include "minunit.h"
#include "search.h"
#include "stats.h"
#include "parser.h"
#include "eval.h"
#include "ttable.h"
#include "board_test.h"
#include "eval_test.h"
#include "parser_test.h"
#include "search_test.h"
#include "stats_test.h"
#include "ttable_test.h"

static bool check_game_end(Board *b) {
    if (b->winner != NOBODY) {
        printf("%s wins!\n", name(b->winner));
        return true;
    } else if (board_full(b)) {
        printf("Game drawn!\n");
        return true;
    } else {
        return false;
    }
}

static char read_column(Board *b) {
    int c = ' ';
    do {
        if (c != '\n') {
            printf("\n[0-6]> ");
        }
        c = getchar();
    } while (c < '0' || c > '6' || board_column_full(b, c - '0'));
    putchar('\n');
    return c;
}

static void play_game() {
    Board b;
    board_init(&b);

    // Reload the transposition table with positions that occur in the opening
    // TODO: check whether this is helpful or not
    if (access("ttable.ser", R_OK)) {
        FILE *openingdb = fopen("ttable.ser", "r");
        ttable_read(openingdb);
        fclose(openingdb);
    }

    Player ai = WHITE;

    while (true) {
        if (b.turn == ai) {
            // put piece selected by the engine
            Variation var;
            variation_init(&var);
            SearchRecord rec;
            searchrecord_init(&rec);
            search(&b, &var, &rec);

#ifdef DEBUG
            stats_print(&b, &var, &rec);
#endif

            board_put(&b, var.moves[0]);
            if (check_game_end(&b)) {
                break;
            }
        } else {
            board_print(&b);
            char c = read_column(&b);

            // put piece selected by the human player
            board_put(&b, c - '0');
            board_print(&b);
            if (check_game_end(&b)) {
                break;
            }
        }
    }

    printf("\nFinal position\n");
    board_print(&b);
}

int main(int argc, char** argv) {
#ifdef BENCHMARK
    FILE *boards = tmpfile();
    benchmark_sample(boards);
    rewind(boards);
    benchmark_run(boards, stdout, stdout);
    fclose(boards);
#endif

#ifdef DEBUG
    board_test();
    eval_test();
    parser_test();
    search_test();
    stats_test();
    ttable_test();
    if (tests_failed > 0) {
        puts("TESTS FAILED!");
        return (EXIT_FAILURE);
    }
#endif

    puts("Welcome to 'Connect Four'!");
    while (true) {
        puts("\nA new game starts!\n");
        play_game();
    }

    return (EXIT_SUCCESS);
}
