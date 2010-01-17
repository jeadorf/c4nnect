#ifndef _BOARD_H
#define	_BOARD_H

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

/* Players. Do not change the values of this enumeration. */
#define WHITE 0
#define BLACK 1
#define NOBODY 2

typedef int8_t Player;

Player other(Player p);

#define NUM_ROWS 6
#define NUM_COLS 7
#define NUM_DIAGS NUM_ROWS + NUM_COLS - 1

/* The board representation. Pieces are stored separately for each side in a
 * bitfield representation similar to the Bitboards used in some chess programs.
 */
struct Board {
    /* Stores all positions of white (columns[WHITE]) and black (columns[BLACK])
     * pieces present on the board (L means present). The first piece that is
     * inserted in a column is represented by the least significant bit of the
     * column value. */
    uint8_t cols[2][NUM_COLS];
    /* Perfect hash value of the board position.
     *
     *   LSB
     * [ 0- 5] column 0,  bit  0 corresponds to row 0, bit 5 to row 5
     * [ 6- 8] number of pieces in column 0, bit  8 MSB
     * [ 9-14] column 1, bit  9 corresponds to row 0
     * [15-17] number of pieces in column 1, bit 17 MSB
     * [18-23] column 2, bit 18 corresponds to row 0
     * [24-26] number of pieces in column 2, bit 26 MSB
     * [27-32] column 3, bit 27 corresponds to row 0
     * [33-35] number of pieces in column 3, bit 35 MSB
     * [36-41] column 4, bit 36 corresponds to row 0
     * [42-44] number of pieces in column 4, bit 44 MSB
     * [45-50] column 5, bit 45 corresponds to row 0
     * [51-53] number of pieces in column 5, bit 53 MSB
     * [54-59] column 6, bit 54 corresponds to row 0
     * [60-62] number of pieces in column 6, bit 62 MSB
     * [63   ] must be set to zero
     *   MSB
     * 0: corresponds to white player
     * 1: corresponds to black player
     * All bit positions that correspond to empty slots must be zero
     *  Example:
     *      - - - - - - -
     *      - - - - - - -
     *      b b - - - - -
     *      w w - - - - -
     *      w w - - - - -
     *      w b b - b - w
     *       (it's BLACK's move)
     *  encodes to
     *  0 (LSB)                                                                  62 63 (MSB)
     *  000100-001-100100-001-100000-100-000000-000-100000-100-000000-000-000000-100-0
     *  In hex this would be
     *  0 LSB                                                                       63 MSB
     *  8    0    3    1    6    0    1    0    0    1    4    0    0    0    0    1
     *  which is printed with the most-significant byte at the left as
     *  0x1000041001061308
     */
    uint64_t code;
    /* Stores the bit of the topmost pieces in each column. 0 means emoty and
     * (1 << NUM_ROWS) */
    int8_t tops[NUM_COLS];
    /* number of moves already done */
    int8_t move_cnt;
    /* The player who gained four in a row, column or diagonal */
    Player winner;
};

typedef struct Board Board;

void board_init(Board *b);

void board_put(Board *b, Player p, int8_t col);

void board_undo(Board *b, int8_t col);

Player board_get(Board *b, int8_t row, int8_t col);

Player board_get_top(Board *b, int8_t col);

Player board_winner(Board *b);

bool board_column_full(Board *b, int8_t col);

bool board_full(Board *b);

void board_hash(Board *b, uint64_t *prim_hash, uint64_t *snd_hash, uint64_t *hash);

/* Reads a binary representation and compiles it into a Board struct object. The
 * board is encoded as in Board.code.
 * @Deprecated just read Board.code
 */
uint64_t board_encode(Board *b);

void board_decode(Board *b, uint64_t n);

void board_print(Board *b);

void board_printd(Board *b, int8_t depth);

#endif	/* _BOARD_H */

