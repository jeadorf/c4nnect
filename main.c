#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "search.h"
#include "minunit.h"
#include "board_test.h"
#include "eval_test.h"
#include "parser_test.h"
#include "search_test.h"
#include "benchmark.h"
#include "search.h"

static void play_game() {
    Board b;
    board_init(&b);
    
    char c, d;
    while (true) {
        board_print(&b);

        do {
            printf("[0-6] > ");
            c = getchar();
            while (c == '\n') {
                putchar('\b');
            }
            d = getchar();
            if (d != '\n') {
                while ((d = getchar()) != '\n') {
                    putchar('\b');
                }
                c = -1;
                continue;
            }
            if (board_column_full(&b, (c - '0'))) {
                printf("You can no longer put a piece in this column!\n");
            }
        } while (c < 0 || c < '0' || c > '6' || board_column_full(&b, c - '0'));

        // put piece selected by the human player
        board_put(&b, c - '0');
        board_print(&b);

        if (b.winner == WHITE) {
            board_print(&b);
            printf("You win!\n");
            break;
        } else if (board_full(&b)) {
            board_print(&b);
            printf("Game drawn!\n");
            break;
        }

        // put piece selected by the engine
        int8_t col = searchm(&b);
        board_put(&b, col);
        if (b.winner == BLACK) {
            board_print(&b);
            printf("Computer wins!\n");
            break;
        } else if (board_full(&b)) {
            board_print(&b);
            printf("Game drawn!\n");
            break;
        }
    }
}

int main(int argc, char** argv) {
#ifdef BENCHMARK
    FILE *boards_out = fopen("boards", "w");
    benchmark_sample(boards_out);
    fclose(boards_out);

    FILE *boards_in = fopen("boards", "r");
    FILE *eval_out = fopen("eval.csv", "w");
    FILE *summ_out = fopen("summ.csv", "w");
    benchmark_run(boards_in, eval_out, summ_out);
    fclose(boards_in);
    fclose(eval_out);
    fclose(summ_out);
#endif

#ifdef DEBUG
    // TODO: Fix tests such that they will show a summary at the end and exit on failure
    board_test();
    eval_test();
    search_test();
    parser_test();
    if (tests_failed > 0) {
        printf("TESTS FAILED!");
    }
#endif

    puts("Welcome to 'Connect Four'!");
    while (true) {
        puts("A new game starts!\n");
        play_game();
    }

    return (EXIT_SUCCESS);
}
