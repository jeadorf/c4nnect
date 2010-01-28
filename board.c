#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include "board.h"
#include "util.h"

/* Returns the number of pieces in the c-th column of a board with code n. You
 * MUST NOT call this macro with parameters that contain side-effects. */
#define GET_TOP(n, c) (((n) >> (NUM_ROWS + (c) * 9)) & 0x7)
/* Encodes the number of pieces in the c-th column of a board with code n. You
 * MUST NOT call this macro with parameters that contain side-effects. */
#define SET_TOP(n, c, t) (((n) & ~(((uint64_t) 0x7) << (NUM_ROWS + (c) * 9))) | (((uint64_t) (t) << (NUM_ROWS + (c) * 9))))
/* Returns the bit at row r and column c of a board with code n. You
 * MUST NOT call this macro with parameters that contain side-effects. */
#define GET(n, r, c) ((n) >> ((c) * 9 + (r)) & 1)
/* Sets or deletes a bit at row r and column c of a board with code n. You
 * MUST NOT call this macro with parameters that contain side-effects. */
#define SET(n, r, c, p) (((n) & ~(((uint64_t) 1) << ((c) * 9 + (r)))) | (((uint64_t) (p)) << ((c) * 9 + (r))))
/* Get the player to move */
#define GET_TURN(n) ((n) >> 63)
/* Set the player to move */
#define SET_TURN(n, p) ((n) ^ (((uint64_t) 1) << 63) | ((uint64_t) (p)) << 63)
/* Set the opponent to be the player to move */
#define OTHER(n) ((n) ^ (((uint64_t) 1) << 63))

inline Player other(Player p) {
    return p == WHITE ? BLACK : WHITE;
}

void board_init(Board *b) {
    memset(b, 0, sizeof (Board));
    b->winner = NOBODY;
}

static bool board_move_wins_col(Board *b, Player p, int8_t row, int8_t col) {
    return ((row >= 3) && (((b->cols[p][col] >> (row - 3)) ^ 0xF) == 0));
}

static bool board_move_wins_row(Board *b, Player p, int8_t row, int8_t col) {
    register int8_t count = 0;
    // Walk to the left maximum three steps and as long as the pieces match
    // the player, increment the line counter.
    register int8_t currentcol = col;
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

static bool board_move_wins_diagup(Board *b, Player p, int8_t row, int8_t col) {
    register int8_t count = 0;
    // Walk left down for maximum three steps and as long as the pieces match the
    // player increment the line counter.
    register int8_t currentrow = row;
    register int8_t currentcol = col;
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

static bool board_move_wins_diagdown(Board *b, Player p, int8_t row, int8_t col) {
    register int8_t count = 0;
    // Walk left up for maximum three steps and as long as the pieces match the
    // player increment the line counter.
    register int8_t currentrow = row;
    register int8_t currentcol = col;
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
            && currentrow + 3 >= row
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

static bool board_move_wins(Board *b, Player p, int8_t row, int8_t col) {
    return board_move_wins_row(b, p, row, col)
            || board_move_wins_col(b, p, row, col)
            || board_move_wins_diagup(b, p, row, col)
            || board_move_wins_diagdown(b, p, row, col);
}

inline void board_put(Board *b, int8_t col) {
    board_put_forced(b, b->turn, col);
}

void board_put_forced(Board *b, Player p, int8_t col) {
#if DEBUG
    if (col < 0 || col >= NUM_COLS) {
        handle_error("column out of range");
    }
#endif
    
    int8_t row = b->tops[col];
    if (row == NUM_ROWS) {
        handle_error("Column is already full, cannot put another piece in this column!");
    }

    // Put a piece on the column
    b->cols[p][col] |= (1 << row);
    b->tops[col]++;
    b->turn = other(p);

    // Update perfect hash
    int nr = row + 1;
    b->code = SET_TOP(b->code, col, nr);
    b->code = SET(b->code, row, col, p);
    b->code = OTHER(b->code);

    if (board_move_wins(b, p, row, col)) {
        b->winner = p;
    }
    b->move_cnt++;
}

void board_undo(Board *b, int8_t col) {
    int8_t row = b->tops[col] - 1;
    Player p = board_get(b, row, col);
    b->winner = NOBODY;
    b->turn = other(b->turn);

    // Kill bit at row
    b->cols[p][col] ^= (1 << row);
    b->tops[col]--;

    // Restore perfect hash by decrementing top counter and zeroing out the bit
    // that might be set at row,col.
    // b->code = CLEAR_TOP(b->code, col);
    b->code = SET_TOP(b->code, col, row);
    b->code = SET(b->code, row, col, 0);
    b->code = OTHER(b->code);

    b->move_cnt--;
}

Player board_get(Board *b, int8_t row, int8_t col) {
    // no faster 2 - (((b->cols[WHITE][col] >> row) & 1) << 1) - ((b->cols[BLACK][col] >> row) & 1);
    if (b->cols[WHITE][col] & (1 << row)) {
        return WHITE;
    } else if (b->cols[BLACK][col] & (1 << row)) {
        return BLACK;
    } else {
        return NOBODY;
    }
}

Player board_get_top(Board *b, int8_t col) {
    return board_get(b, b->tops[col - 1], col);
}

bool board_full(Board *b) {
    return b->move_cnt >= 42;
}

bool board_column_full(Board *b, int8_t col) {
    return b->tops[col] == NUM_ROWS;
}

uint64_t board_encode(Board *b) {
    uint64_t n = 0UL;
    int8_t top = 0UL;
    Player p;
    for (int8_t c = 0; c < NUM_COLS; c++) {
        // Save number of pieces in this column
        top = b->tops[c];
        n = SET_TOP(n, c, top);
        for (int8_t r = 0; r < top; r++) {
            // Must be careful not to shift bits out at the right (MSB) of
            // an integer value, have to specify UL explicitly.
            p = board_get(b, r, c);
            n = SET(n, r, c, p);
            n = OTHER(n);
        }
    }
    return n;
}

void board_decode(Board *b, uint64_t n) {
    // See also parser_fread
    for (int8_t c = 0; c < NUM_COLS; c++) {
        // Extract number of pieces in this column
        int8_t top = GET_TOP(n, c);
        for (int8_t r = 0; r < top; r++) {
            if (GET(n, r, c) & 1) {
                board_put_forced(b, BLACK, c);
            } else {
                board_put_forced(b, WHITE, c);
            }
        }
    }
    b->turn = GET_TURN(n);
}

void board_print(Board *b) {
    board_printd(b, 0);
}

void board_printd(Board *b, int8_t depth) {
    int32_t r, c; //
    for (r = NUM_ROWS - 1; r >= 0; r--) {
        for (c = 0; c < depth * 4; c++) {
            putchar(' ');
        }
        putchar('"');
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
        putchar('"');
        putchar('\n');
    }

    putchar('\n');
}
