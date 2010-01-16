// fmemopen seems to be a function that is some GNU extension to the C library
#define _GNU_SOURCE

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "util.h"

// TODO: fix problem with modifications to NUM_COL (and test cases and so on)
void parser_read(Board *b, char *data) {
    FILE *stream;
    stream = fmemopen(data, strlen(data), "r");
    parser_fread(b, stream);
    fclose(stream);
}

void parser_fread(Board *b, FILE *stream) {
    Player pieces[NUM_ROWS][NUM_COLS];
    memset(pieces, 0, NUM_ROWS * NUM_COLS);

    // We start at the top left corner, walking along the data, keeping track
    // of which row and column information is expected. We stop either at the
    // end of string (error) or when all data is read.
    int row = NUM_ROWS - 1;
    int col = 0;
    Player p;
    int d;
    do {
        d = fgetc(stream);
        if (d == EOF) {
            handle_error("Reached end of file/buffer, board data was not complete.");
        } else if (isspace(d) || d == '\"') {
            continue;
        } else {
            switch (d) {
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
