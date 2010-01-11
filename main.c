#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "board_test.h"
#include "eval_test.h"
#include "parser.h"
#include "search.h"
#include "search_test.h"
#include "util.h"
#include "parser_test.h"

/*
 * 
 */
int main(int argc, char** argv) {
    // FIXME: remove tests before release
    board_test();
    eval_test();
    search_test();
    parser_test();

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
        board_put(&b, BLACK, c);
        board_print(&b);

        if (b.winner == BLACK) {
            board_print(&b);
            printf("You win!\n");
            break;
        } else if (board_full(&b)) {
            board_print(&b);
            printf("Game drawn!\n");
            break;
        }

        // put piece selected by the engine
        int col = search(&b, WHITE);
/*
        printf("after selecting black piece %d\n", col);
        board_print(&b);
*/

        board_put(&b, WHITE, col);
/*
        printf("after putting black piece\n");
        board_print(&b);
*/
        if (b.winner == WHITE) {
            board_print(&b);
            printf("Computer wins!\n");
            break;
        } else if (board_full(&b)) {
            board_print(&b);
            printf("Game drawn!\n");
            break;
        }
    }
    return (EXIT_SUCCESS);
}

