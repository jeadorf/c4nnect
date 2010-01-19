#include "board2eps.h"
#include "board.h"
#include "util.h"

int board2eps(Board *b, FILE *eps_out) {
    // TODO: where to read template from?
    FILE *template_in = fopen("board.eps", "r");
    if (template_in == NULL) {
        handle_error("Cannot read PostScript board template file");
    }

    // TODO: improve copy procedure
    char c;
    while ((c = getc(template_in)) != EOF) {
        putc(c, eps_out);
    }
    fclose(template_in);
    fputs("\n% board data\n", eps_out);
    for (int c = 0; c < NUM_COLS; c++) {
        for (int r = 0; r < NUM_ROWS; r++) {
            // FIXME: fix board_get and board_get_top
            if (board_get(b, r, c) == WHITE) {
                fprintf(eps_out, "%d %d putwhite\n", r, c);
            } else if (board_get(b, r, c) == BLACK) {
                fprintf(eps_out, "%d %d putblack\n", r, c);
            }
        }
    }

    if (b->winner == WHITE) {
        fputs("whitewins\n", eps_out);
    } else if (b->winner == BLACK) {
        fputs("blackwins\n", eps_out);
    } else if (board_full(b)) {
        fputs("draw\n", eps_out);
    }
}