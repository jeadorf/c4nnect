#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "board.h"

void board_put(Board *b, Player p, int col) {
    // TODO: exception handling: what if the column is already full?
    // Put a piece on the column
    b->cols[p][col] |= (1 << b->tops[col]);
    b->tops[col]++;
    // Check for win situation
    if (b-> tops[col] >= 4) {
        int k = 1, i;
        for (i = 1; i <= 3; i++) {
            if (board_get(b, b->tops[col]-1-i, col) == p)
                k++;
        }
            printf("You nothiing %d!\n", k);
        if (k == 4)
            printf("You WIN!\n");
    }
}

Player board_get(Board *b, int row, int col) {
    if ((b->cols[WHITE][col] >> row) & 1) {
        return WHITE;
    } else if ((b->cols[BLACK][col] >> row) & 1) {
        return BLACK;
    } else {
        return NOBODY;
    }
}

bool board_full(Board *b, int col) {
    return b->tops[col] == NUM_ROWS;
}

void board_print(Board *b) {
    int r, c;
    for (r = NUM_ROWS - 1; r >= 0; r--) {
        for (c = 0; c < NUM_COLS; c++) {
            switch (board_get(b, r, c)) {
                case WHITE:
                    putchar('w');
                    break;
                case BLACK:
                    putchar('b');
                    break;
                default:
                    putchar('-');
                    break;
            }
            putchar(' ');
        }
        putchar('\n');
    }
    /*
        putchar('\n');
        for (c = 0; c < NUM_COLS; c++) {
            printf("col %d : %02x %02x top %d\n", c, b->cols[0][c], b->cols[1][c], b->tops[c]);
        }
     */
}
