#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "board.h"
#include "board_test.h"
#include "minunit.h"


Board b;

/*
 * 
 */
int main(int argc, char** argv) {
    char c;

    board_test();

    while (true) {
        board_print(&b);
        do {
            putchar('>');
            putchar(' ');
            c = getchar() - '0';
            // Read line feed
            getchar();
        } while (c < 0 || c >= NUM_COLS || board_full(&b, c));
        board_put(&b, WHITE, c);
        board_put(&b, BLACK, 4);
    }
    return (EXIT_SUCCESS);
}

