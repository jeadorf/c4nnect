#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "board.h"
#include "util.h"

/*! Returns the number of pieces in the c-th column of a board with code n. You
 * MUST NOT call this macro with parameters that contain side-effects. */
#define GET_TOP(n, c) (((n) >> (NUM_ROWS + (c) * 9)) & 0x7)
/*! Encodes the number of pieces in the c-th column of a board with code n. You
 * MUST NOT call this macro with parameters that contain side-effects. */
#define SET_TOP(n, c, t) (((n) & ~(((uint64_t) 0x7) << (NUM_ROWS + (c) * 9))) | (((uint64_t) (t) << (NUM_ROWS + (c) * 9))))
/*! Returns the bit at row r and column c of a board with code n. You
 * MUST NOT call this macro with parameters that contain side-effects. */
#define GET(n, r, c) ((n) >> ((c) * 9 + (r)) & 1)
/*! Sets or deletes a bit at row r and column c of a board with code n. You
 * MUST NOT call this macro with parameters that contain side-effects. */
#define SET(n, r, c, p) (((n) & ~(((uint64_t) 1) << ((c) * 9 + (r)))) | (((uint64_t) (p)) << ((c) * 9 + (r))))
/*! Get the player to move. */
#define GET_TURN(n) ((n) >> 63)
/*! Set the player to move. */
#define SET_TURN(n, p) ((n) ^ (((uint64_t) 1) << 63) | ((uint64_t) (p)) << 63)
/*! Set the opponent to be the player to move. */
#define OTHER(n) ((n) ^ (((uint64_t) 1) << 63))

inline Player other(Player p) {
    return p == WHITE ? BLACK : WHITE;
}

inline char* name(Player p) {
    return p == WHITE ? "White" : "Black";
}

/* Bit masks for detection of winning moves */
static uint64_t LEFT3[NUM_ROWS][NUM_COLS];
static uint64_t LEFT2[NUM_ROWS][NUM_COLS];
static uint64_t LEFT1[NUM_ROWS][NUM_COLS];
static uint64_t LEFT0[NUM_ROWS][NUM_COLS];
static uint64_t DOWN3[NUM_ROWS][NUM_COLS];
static uint64_t DIAGA3[NUM_ROWS][NUM_COLS];
static uint64_t DIAGA2[NUM_ROWS][NUM_COLS];
static uint64_t DIAGA1[NUM_ROWS][NUM_COLS];
static uint64_t DIAGA0[NUM_ROWS][NUM_COLS];
static uint64_t DIAGD3[NUM_ROWS][NUM_COLS];
static uint64_t DIAGD2[NUM_ROWS][NUM_COLS];
static uint64_t DIAGD1[NUM_ROWS][NUM_COLS];
static uint64_t DIAGD0[NUM_ROWS][NUM_COLS];

static uint64_t mask_left(int8_t row, int8_t col, int8_t s) {
    if (col - s >= 0 && col - s < NUM_COLS - 3) {
        uint64_t mask = 0UL;
        mask = SET(mask, row, col - s, 1);
        mask = SET(mask, row, col - s + 1, 1);
        mask = SET(mask, row, col - s + 2, 1);
        mask = SET(mask, row, col - s + 3, 1);
        return mask;
    } else {
        return 1UL << 63;
    }
}

static uint64_t mask_down(int8_t row, int8_t col) {
    if (row >= 3) {
        uint64_t mask = 0UL;
        mask = SET(mask, row - 3, col, 1);
        mask = SET(mask, row - 2, col, 1);
        mask = SET(mask, row - 1, col, 1);
        mask = SET(mask, row, col, 1);
        return mask;
    } else {
        return 1UL << 63;
    }
}

static uint64_t mask_diaga(int8_t row, int8_t col, int8_t s) {
    if (row - s >= 0 && row - s + 3 < NUM_ROWS && col - s >= 0 && row - s + 3 < NUM_COLS) {
        uint64_t mask = 0UL;
        mask = SET(mask, row - s, col - s, 1);
        mask = SET(mask, row - s + 1, col - s + 1, 1);
        mask = SET(mask, row - s + 2, col - s + 2, 1);
        mask = SET(mask, row - s + 3, col - s + 3, 1);
        return mask;
    } else {
        // TODO: remove magic number
        return 1UL << 63;
    }
}

static uint64_t mask_diagd(int8_t row, int8_t col, int8_t s) {
    if (row + s - 3 >= 0 && row + s < NUM_ROWS && col - s >= 0 && row - s + 3 < NUM_COLS) {
        uint64_t mask = 0UL;
        mask = SET(mask, row + s, col - s, 1);
        mask = SET(mask, row + s - 1, col - s + 1, 1);
        mask = SET(mask, row + s - 2, col - s + 2, 1);
        mask = SET(mask, row + s - 3, col - s + 3, 1);
        return mask;
    } else {
        // TODO: remove magic number
        return 1UL << 63;
    }
}

static void generate_masks() {
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            LEFT3[row][col] = mask_left(row, col, 3);
            LEFT2[row][col] = mask_left(row, col, 2);
            LEFT1[row][col] = mask_left(row, col, 1);
            LEFT0[row][col] = mask_left(row, col, 0);
            DOWN3[row][col] = mask_down(row, col);
            DIAGA3[row][col] = mask_diaga(row, col, 3);
            DIAGA2[row][col] = mask_diaga(row, col, 2);
            DIAGA1[row][col] = mask_diaga(row, col, 1);
            DIAGA0[row][col] = mask_diaga(row, col, 0);
            DIAGD3[row][col] = mask_diagd(row, col, 3);
            DIAGD2[row][col] = mask_diagd(row, col, 2);
            DIAGD1[row][col] = mask_diagd(row, col, 1);
            DIAGD0[row][col] = mask_diagd(row, col, 0);
        }
    }
}

void board_init(Board *b) {
    memset(b, 0, sizeof (Board));
    b->winner = NOBODY;
    generate_masks();
}

static bool board_move_wins_col(Board *b, Player p, int8_t row, int8_t col) {
    return (b->pos[p] & DOWN3[row][col]) == DOWN3[row][col];
}

bool board_move_wins_row(Board *b, Player p, int8_t row, int8_t col) {
    return ((b->pos[p] & LEFT3[row][col]) == LEFT3[row][col]) ||
           ((b->pos[p] & LEFT2[row][col]) == LEFT2[row][col]) ||
           ((b->pos[p] & LEFT1[row][col]) == LEFT1[row][col]) ||
           ((b->pos[p] & LEFT0[row][col]) == LEFT0[row][col]);
}

static bool board_move_wins_diagup(Board *b, Player p, int8_t row, int8_t col) {
    return ((b->pos[p] & DIAGA3[row][col]) == DIAGA3[row][col]) ||
           ((b->pos[p] & DIAGA2[row][col]) == DIAGA2[row][col]) ||
           ((b->pos[p] & DIAGA1[row][col]) == DIAGA1[row][col]) ||
           ((b->pos[p] & DIAGA0[row][col]) == DIAGA0[row][col]);
}

static bool board_move_wins_diagdown(Board *b, Player p, int8_t row, int8_t col) {
    return ((b->pos[p] & DIAGD3[row][col]) == DIAGD3[row][col]) ||
           ((b->pos[p] & DIAGD2[row][col]) == DIAGD2[row][col]) ||
           ((b->pos[p] & DIAGD1[row][col]) == DIAGD1[row][col]) ||
           ((b->pos[p] & DIAGD0[row][col]) == DIAGD0[row][col]);
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

    b->pos[p] = SET(b->pos[p], row, col, 1);
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
    b->pos[p] = SET(b->pos[p], row, col, 0);
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
    if (GET(b->pos[WHITE], row, col)) {
        return WHITE;
    } else if (GET(b->pos[BLACK], row, col)) {
        return BLACK;
    } else {
        return NOBODY;
    }
}

inline Player board_get_top(Board *b, int8_t col) {
    int8_t top = b->tops[col];
    if (top > 0) {
        return board_get(b, top - 1, col);
    } else {
        return NOBODY;
    }
}

inline bool board_full(Board *b) {
    return b->move_cnt >= 42;
}

inline bool board_column_full(Board *b, int8_t col) {
    return b->tops[col] == NUM_ROWS;
}

void board_decode(Board *b, uint64_t n) {
    board_init(b);
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

void board_print(Board * b) {
    board_printd(b, 0);
}

void board_printd(Board *b, int8_t depth) {
    int32_t r, c;
    for (r = NUM_ROWS - 1; r >= -2; r--) {
        for (c = 0; c < depth * 4; c++) {
            putchar(' ');
        }
        if (r >= 0) {
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
        } else if (r == -1) {
            putchar(' ');
            for (c = 0; c < NUM_COLS; c++) {
                putchar('"');
                putchar(' ');
            }
            putchar(' ');
        } else {
            putchar(' ');
            for (c = 0; c < NUM_COLS; c++) {
                putchar('0' + c);
                putchar(' ');
            }
            putchar(' ');
        }
        putchar('\n');
    }
    putchar('\n');
}
