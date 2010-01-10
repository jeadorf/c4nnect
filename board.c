#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "board.h"

#define max(A,B) (((A)>(B))?(A):(B))
#define min(A,B) (((A)<(B))?(A):(B))

void board_init(Board *b) {
    memset(b, 0, sizeof(Board));
    b->winner = NOBODY;
}

static bool board_move_wins_col(Board *b, Player p, int row, int col) {
    int count = 0;
    if (row >= 3) {
        int start = row - 3;
        int end = row;
        for (int i = start; i <= end; i++) {
            if (board_get(b, i, col) == p) {
                count++;
            } else {
                break;
            }
        }
    }
    return count >= 4;
}

static bool board_move_wins_row(Board *b, Player p, int row, int col) {
    int count = 0;
    // Walk to the left maximum three steps and as long as the pieces match
    // the player, increment the line counter.
    int currentcol = col;
    while (currentcol >= 0
            && currentcol >= col - 3
            && p == board_get(b, row, currentcol)) {
        count++;
        currentcol--;
    }
    // Walk to the right maximum three steps and as long as the pieces match
    // the player, increment the line counter. We do not start with the piece
    // at (row, col) here such that it is not counted twice.
    currentcol = col + 1;
    while (currentcol < NUM_COLS
            && currentcol <= col + 3
            && p == board_get(b, row, currentcol)) {
        count++;
        currentcol++;
    }
    return count >= 4;
}

static bool board_move_wins_diagup(Board *b, Player p, int row, int col) {
    int count = 0;
    // Walk left down for maximum three steps and as long as the pieces match the
    // player increment the line counter.
    int currentrow = row;
    int currentcol = col;
    while (currentcol >= 0
            && currentcol >= col - 3
            && currentrow >= 0
            && currentrow >= row - 3
            && p == board_get(b, currentrow, currentcol)) {
        count++;
        currentcol--;
        currentrow--;
    }
    // Now walk right up for maximum three steps, doing the same again just
    // that we do not start at (row, col).
    currentrow = row + 1;
    currentcol = col + 1;
    while (currentcol < NUM_COLS
            && currentcol <= col + 3
            && currentrow < NUM_ROWS
            && currentrow <= row + 3
            && p == board_get(b, currentrow, currentcol)) {
        count++;
        currentcol++;
        currentrow++;
    }
    return count >= 4;
}

static bool board_move_wins_diagdown(Board *b, Player p, int row, int col) {
    int count = 0;
    // Walk left up for maximum three steps and as long as the pieces match the
    // player increment the line counter.
    int currentrow = row;
    int currentcol = col;
    while (currentcol >= 0
            && currentcol >= col - 3
            && currentrow < NUM_ROWS
            && currentrow <= row + 3
            && p == board_get(b, currentrow, currentcol)) {
        count++;
        currentcol--;
        currentrow++;
    }
    // Now walk right down for maximum three steps, doing the same again just
    // that we do not start at (row, col).
    currentrow = row - 1;
    currentcol = col + 1;
    while (currentcol < NUM_COLS
            && currentcol <= col + 3
            && currentrow >= 0
            && currentrow >= row - 3
            && p == board_get(b, currentrow, currentcol)) {
        count++;
        currentcol++;
        currentrow--;
    }
    return count >= 4;
}

// Check for win situation. The new piece must have been involved in a
// win line. Thus, we just need to check rows, columns and diagonals
// starting from the new piece.
static bool board_move_wins(Board *b, Player p, int row, int col) {
    return board_move_wins_row(b, p, row, col)
            || board_move_wins_col(b, p, row, col)
            || board_move_wins_diagup(b, p, row, col)
            || board_move_wins_diagdown(b, p, row, col);
}

void board_put(Board *b, Player p, int col) {
    // TODO: exception handling: what if the column is already full?
    // Put a piece on the column
    int row = b->tops[col];
    b->cols[p][col] |= (1 << row);
    b->tops[col]++;

    if (board_move_wins(b, p, row, col)) {
        b->winner = p;
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
