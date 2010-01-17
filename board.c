#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include "board.h"
#include "util.h"

inline Player other(Player p) {
    return p == WHITE ? BLACK : WHITE;
}

void board_init(Board *b) {
    // TODO: do board initialization in a safe manner
    memset(b, 0, sizeof (Board));
    b->winner = NOBODY;
}

static bool board_move_wins_col(Board *b, Player p, uint8_t row, uint8_t col) {
    return ((row -= 3) >= 0) && (((b->cols[p][col] >> row) ^ 0xF) == 0);
}

static bool board_move_wins_row(Board *b, Player p, uint8_t row, uint8_t col) {
    register uint8_t count = 0;
    // Walk to the left maximum three steps and as long as the pieces match
    // the player, increment the line counter.
    register uint8_t currentcol = col;
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

static bool board_move_wins_diagup(Board *b, Player p, uint8_t row, uint8_t col) {
    register uint8_t count = 0;
    // Walk left down for maximum three steps and as long as the pieces match the
    // player increment the line counter.
    register uint8_t currentrow = row;
    register uint8_t currentcol = col;
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

static bool board_move_wins_diagdown(Board *b, Player p, uint8_t row, uint8_t col) {
    register uint8_t count = 0;
    // Walk left up for maximum three steps and as long as the pieces match the
    // player increment the line counter.
    register uint8_t currentrow = row;
    register uint8_t currentcol = col;
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

static bool board_move_wins(Board *b, Player p, uint8_t row, uint8_t col) {
    return board_move_wins_row(b, p, row, col)
            || board_move_wins_col(b, p, row, col)
            || board_move_wins_diagup(b, p, row, col)
            || board_move_wins_diagdown(b, p, row, col);
}

void board_put(Board *b, Player p, uint8_t col) {
    uint8_t row = b->tops[col];
    if (row == NUM_ROWS) {
        handle_error("Column is already full, cannot put another piece in this column!");
    }

    // Put a piece on the column
    b->cols[p][col] |= (1 << row);
    b->tops[col]++;

    if (board_move_wins(b, p, row, col)) {
        b->winner = p;
    }
    b->move_cnt++;
}

void board_undo(Board *b, uint8_t col) {
    uint8_t row = b->tops[col] - 1;
    Player p = board_get(b, row, col);
    b->winner = NOBODY;
    // Kill bit at row
    b->cols[p][col] ^= (1 << row);
    b->tops[col]--;
    b->move_cnt--;
}

Player board_get(Board *b, uint8_t row, uint8_t col) {
    // no faster 2 - (((b->cols[WHITE][col] >> row) & 1) << 1) - ((b->cols[BLACK][col] >> row) & 1);
    if (b->cols[WHITE][col] & (1 << row)) {
        return WHITE;
    } else if (b->cols[BLACK][col] & (1 << row)) {
        return BLACK;
    } else {
        return NOBODY;
    }
}

Player board_get_top(Board *b, uint8_t col) {
    return board_get(b, b->tops[col - 1], col);
}

bool board_full(Board *b) {
    uint8_t c;
    for (c = 0; c < NUM_COLS; c++) {
        if (!board_column_full(b, c)) {
            return false;
        }
    }
    return true;
}

bool board_column_full(Board *b, uint8_t col) {
    return b->tops[col] == NUM_ROWS;
}

void board_hash(Board *b, uint64_t *prim_hash, uint64_t *snd_hash, uint64_t *hash) {
    *hash = 0UL;
    *prim_hash = 0UL;
    *snd_hash = 0UL;
    uint8_t c;
    for (c = 0; c < NUM_COLS; c++) {
        *prim_hash |= ((uint64_t) b->cols[WHITE][c]) << (c * NUM_ROWS);
        *snd_hash |= ((uint64_t) b->cols[BLACK][c]) << (c * NUM_ROWS);
    }
    *hash = (*prim_hash * *prim_hash) ^ (*snd_hash * *snd_hash);
}

uint64_t board_encode(Board *b) {
    uint64_t n = 0UL;
    uint64_t top = 0UL;
    for (uint8_t c = 0; c < NUM_COLS; c++) {
        // Save number of pieces in this column
        top = b->tops[c];
        n |= top << (NUM_ROWS + c * 9);
        for (uint8_t r = 0; r < top; r++) {
            if (board_get(b, r, c) == BLACK) {
                // Must be careful not to shift bits out at the right (MSB) of
                // an integer value, have to specify UL explicitly.
                n |= (((uint64_t) 1) << (c * 9 + r));
            }
        }
    }
    return n;
}

void board_decode(Board *b, uint64_t n) {
    for (uint8_t c = 0; c < NUM_COLS; c++) {
        // Extract number of pieces in this column
        uint8_t top = (n >> (NUM_ROWS + c * 9)) & 0x7;
        for (uint8_t r = 0; r < top; r++) {
            if (n >> (c * 9 + r) & 1) {
                board_put(b, BLACK, c);
            } else {
                board_put(b, WHITE, c);
            }
        }
    }
}

void board_print(Board *b) {
    board_printd(b, 0);
}

void board_printd(Board *b, uint8_t depth) {
    int r, c; //
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

#if DEBUG
    putchar('\n');
    uint64_t prim_hash, snd_hash, hash;
    board_hash(b, &prim_hash, &snd_hash, &hash);
    printf("%18s : ", "Primary hash");
    print_uint64_rev(prim_hash);
    printf("%18s : ", "Secondary hash");
    print_uint64_rev(snd_hash);
    printf("%18s : ", "Combined hash");
    print_uint64_rev(hash);
#endif

    putchar('\n');
}
