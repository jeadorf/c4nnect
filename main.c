#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "parser.h"
#include "search.h"
#include "util.h"
#include "board_test.h"
#include "eval_test.h"
#include "parser_test.h"
#include "search_test.h"
#include "benchmark.h"
#include "eval.h"

static void play_game() {
    Board b;
    board_init(&b);
    char c, d;
    while (true) {
        board_print(&b);

        do {
            printf("[%d-%d] > ", 1, NUM_COLS);
            c = getchar() - '1';
            if (c == '\n' - '1') {
                c = -1;
                continue;
            }
            d = getchar();
            if (d != '\n') {
                while ((d = getchar()) != '\n') {
                    // wait
                }
                c = -1;
                continue;
            }
            if (board_column_full(&b, c)) {
                printf("You can no longer put a piece in this column!\n");
            }
        } while (c < 0 || c >= NUM_COLS || board_column_full(&b, c));

        // put piece selected by the human player
        board_put(&b, c);
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

/*
 * 
 */
int main(int argc, char** argv) {
#if DEBUG
    /*FILE *boards_in = fopen("boards", "r");
    FILE *eval_out = fopen("eval.csv", "w");
    FILE *summ_out = fopen("summ.csv", "w");
    benchmark_run(boards_in, eval_out, summ_out);
    fclose(boards_in);
    fclose(eval_out);
    fclose(summ_out);*/

    board_test();
    eval_test();
    search_test();
    parser_test();
#endif

    puts("Welcome to 'Four in a Row'!");
    while (true) {
        puts("A new game starts!");
        play_game();
    }

    return (EXIT_SUCCESS);
}

