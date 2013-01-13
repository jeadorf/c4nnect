#ifndef _BOARD_H
#define _BOARD_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

/* Players. Do not change the values of these definitions; WHITE and BLACK are
 * used as single bits. */
#define WHITE 0
#define BLACK 1
#define NOBODY 2

typedef int8_t Player;

Player other(Player p);

char* name(Player p);

// The number of rows and columns MUST NOT be changed (e.g. the perfect hashing
// onto a 64-bit integer relies on a 6x7 board.
#define NUM_ROWS 6
#define NUM_COLS 7

/*! Represents a position in the game. Theoretically, a position fits well into
 * a 64-bit long integer (see Board.code). Yet, to make it easier to extract
 * information from the board, some parts of the board data structure contain
 * duplicate information.
 */
struct Board {
    /*! Positions separated by side. The positions of white (pos[WHITE]) and black
     * (pos[BLACK]) are stored in bitboards. The encoding used for the position
     * is similar to Board.code except that the number of pieces is not included
     * and that both white and black pieces are represented by bit 1 in their
     * respective bitboards. See #GET and #SET for further details. */
    uint64_t pos[2];
    /*! Gödel number of the current board position.
     *
     *   LSB \n
     * [ 0- 5] column 0,  bit  0 corresponds to row 0, bit 5 to row 5\n
     * [ 6- 8] number of pieces in column 0, bit  8 MSB\n
     * [ 9-14] column 1, bit  9 corresponds to row 0\n
     * [15-17] number of pieces in column 1, bit 17 MSB\n
     * [18-23] column 2, bit 18 corresponds to row 0\n
     * [24-26] number of pieces in column 2, bit 26 MSB\n
     * [27-32] column 3, bit 27 corresponds to row 0\n
     * [33-35] number of pieces in column 3, bit 35 MSB\n
     * [36-41] column 4, bit 36 corresponds to row 0\n
     * [42-44] number of pieces in column 4, bit 44 MSB\n
     * [45-50] column 5, bit 45 corresponds to row 0\n
     * [51-53] number of pieces in column 5, bit 53 MSB\n
     * [54-59] column 6, bit 54 corresponds to row 0\n
     * [60-62] number of pieces in column 6, bit 62 MSB\n
     * [63   ] 0 if it is WHITE, 1 if it is BLACK to move\n
     *
     *   MSB\n
     * 0: corresponds to white player\n
     * 1: corresponds to black player\n
     *
     * All bit positions that correspond to empty slots must be zero.
     *
     *  Example:
     * \verbatim
     *      - - - - - - -
     *      - - - - - - -
     *      b b - - - - -
     *      w w - - - - -
     *      w w - - - - -
     *      w b b - b - w
     *
     *     (it's BLACK's move)
     * \endverbatim
     *  encodes to
     * \verbatim
     *  0 LSB                                                                    62 63 MSB
     *  000100-001-100100-001-100000-100-000000-000-100000-100-000000-000-000000-100-1
     *
     *  0 LSB                                                                       63 MSB
     *  8    0    3    1    6    0    1    0    0    1    4    0    0    0    0    9
     * \endverbatim
     *  This corresponds to the hexadecimal notation of 0x9000041001061308, where
     * the most significant byte is at the left.
     */
    uint64_t code;
    /*! Number of pieces in each column. */
    int8_t tops[NUM_COLS];
    /*! Number of moves already done. */
    int8_t move_cnt;
    /*! The player to move. */
    Player turn;
    /*! Winner of this game. If the game is still ongoing the winner is set to
     * NOBODY. */
    Player winner;
    /*! IBEF rating (Intuitive Board Evaluation Function) from "Synthesizing
     * Board Evaluation Functions for Connect4 using Machine Learning Techniques"
     * (Martin Stenmark). Fast, simple but quite stupid evaluation based on
     * weighting squares. */
    int ibef_rating;
};

typedef struct Board Board;

/*! Initializes a board. This procedure should be applied to all newly allocated
 * boards. Also, you can reset the board with it. */
void board_init(Board *b);

/*! Adds a piece to the specified column. The color of the piece is defined by
 * the player whose turn it is. */
void board_put(Board *b, int8_t col);

/*! Puts a piece of a certain Player into the col-th column. This
 * might cause the board to e.g. have more black pieces than possible in a
 * normal game. It might also lead to situations that seem valid on the first
 * glance but are impossible in real games. After the invocation, it is the
 * other player's (other(p)) turn. Be careful if you use this method, as it
 * breaks the natural undo mechanism */
void board_put_forced(Board *b, Player p, int8_t col);

/*! Reverts a move by removing the top-most piece in the specified column. */
void board_undo(Board *b, int8_t col);

/*! Gets the owner of the piece at the specified square. If empty, #NOBODY is
 * returned. */
Player board_get(Board *b, int8_t row, int8_t col);

/*! Gets the owner of the top-most piece in the specified column. If empty,
 * #NOBODY is returned. */
Player board_get_top(Board *b, int8_t col);

/*! Checks whether a piece at the specified location would complete a line. */
bool board_is_threat(Board *b, Player p, int8_t row, int8_t col);

/*! Detects whether the specified column is full. */
bool board_column_full(Board *b, int8_t col);

/*! Detects whether all columns are full and the game is drawn. */
bool board_full(Board *b);

/*! Reconstructs a complete board representation from the given Gödel number,
 * such that board_decode(*b, b.code) is a no-op.
 */
void board_decode(Board *b, uint64_t n);

/*! Prints a textual representation of the board. The quoted parts of the string
 * representation are compatible with #parser_read, making it easy to import a
 * board position into code. */
void board_print(Board *b);

/*! Same as board_print, just with indentation. */
void board_printd(Board *b, int8_t depth);

#endif	/* _BOARD_H */

