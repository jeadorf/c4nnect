#ifndef _BOARD_H
#define	_BOARD_H

#include <sys/types.h>
#include <stdbool.h>

/* Players. Do not change the values of this enumeration. */
enum Player {
    WHITE=0,
    BLACK=1,
    NOBODY=2
};

typedef enum Player Player;

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
     * column value, the left-most column is represented by the least significant
     * bit of the row value. */
    ushort cols[2][NUM_COLS];
    ushort rows[2][NUM_ROWS];
    /* Stores the number of pieces in each column (0 means empty) */
    ushort tops[NUM_COLS];
    /* The player who gained four in a row, column or diagonal */
    Player winner;
};

typedef struct Board Board;

void board_put(Board *b, Player p, int col);

Player board_get(Board *b, int row, int col);

Player board_winner(Board *b);

bool board_full(Board *b, int col);

void board_print(Board *b);

#endif	/* _BOARD_H */
