// fmemopen seems to be a function that is some GNU extension to the C library
#define _GNU_SOURCE

#include <ctype.h>
#include <stdbool.h>
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
    memset(pieces, 0, sizeof (pieces));

    // We start at the top left corner, walking along the data, keeping track
    // of which row and column information is expected. We stop either at the
    // end of string (error) or when all data is read.
    int row = NUM_ROWS - 1;
    int col = 0;
    Player p = NOBODY;
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

    // TODO: validate board, count black and white pieces

    // Now replay board. As long as we proceed from bottom to top, it does not
    // matter which piece in the current row we put. The player turn changes
    // a constant number of times, no matter in which order the moves are
    // replayed (in theory). In practice board_put_forced will change the player
    // relatively after each turn, depending on who made the move, so we have
    // to count ourselves.
    board_init(b);
    Player turn = WHITE;
    for (row = 0; row < NUM_ROWS; row++) {
        for (col = 0; col < NUM_COLS; col++) {
            p = pieces[row][col];
            if (p != NOBODY) {
                board_put_forced(b, p, col);
                turn = other(turn);
            }
        }
    }
    // Forcibly set turn, this is sort of a workaround
    b->turn = turn;
}
