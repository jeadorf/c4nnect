#include "parser.h"
#include "util.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void parser_read(Board *b, char *data) {
    Player pieces[NUM_ROWS][NUM_COLS];
    memset(pieces, 0, NUM_ROWS * NUM_COLS);

    // We start at the top left corner, walking along the data, keeping track
    // of which row and column information is expected. We stop either at the
    // end of string (error) or when all data is read.
    int row = NUM_ROWS - 1;
    int col = 0;
    Player p;
    do {
        if (data == '\0') {
            handle_error("Reached end of string, board data was not complete.");
        } else if (isspace(*data)) {
            data++;
            continue;
        } else {
            // TODO: replace with hash-table?
            switch (*data) {
                case 'w':
                case 'W':
                    p = WHITE;
                    break;
                case 'b':
                case 'B':
                    p = BLACK;
                    break;
                case '-':
                    p = NOBODY;
                    break;
                default:
                    handle_error("Illegal character in board data.");
            }
            pieces[row][col] = p;
            data++;
            col++;
            if (col == NUM_COLS) {
                col = 0;
                row--;
            }
        }
    } while (row >= 0);

    // now replay board
    board_init(b);

    for (row = 0; row < NUM_ROWS; row++) {
        for (col = 0; col < NUM_COLS; col++) {
            p = pieces[row][col];
            if (p != NOBODY) {
                board_put(b, p, col);
            }
        }
    }
}
