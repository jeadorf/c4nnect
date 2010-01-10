#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "board.h"
#include "parser.h"
#include "board_test.h"
#include "util.h"

/*
 * 
 */
int main(int argc, char** argv) {
    // FIXME: remove tests before release
    board_test();

    puts("Welcome to 'Four in a Row'!");
    Board b;
    board_init(&b);

    char c;
    while (true) {
        board_print(&b);

        do {
            printf("[%d-%d] > ", 0, NUM_COLS - 1);
            c = getchar() - '0';
            // Read line feed
            getchar();
            if (board_column_full(&b, c)) {
                printf("You can no longer put a piece in this column!\n");
            }
        } while (c < 0 || c >= NUM_COLS || board_column_full(&b, c));

        // put piece selected by the human player
        board_put(&b, WHITE, c);
        if (b.winner == WHITE) {
            board_print(&b);
            printf("You win!\n");
            break;
        }

        // put piece selected by the engine
        board_put(&b, BLACK, rand() % NUM_COLS);
        if (b.winner == BLACK) {
            board_print(&b);
            printf("Computer wins!\n");
            break;
        }
    }
    return (EXIT_SUCCESS);
}

