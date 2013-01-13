#include "board2eps.h"
#include "board.h"
#include "configure.h"
#include "util.h"

#define EPS_BUFFER_SIZE 4096

 void fboard2eps(Board *b, char *filename) {
    FILE *eps_out = fopen(filename, "w");
    board2eps(b, eps_out);
    fclose(eps_out);
}

void board2eps(Board *b, FILE *eps_out) {
    FILE *template_in = fopen(C4NNECT_DIR "/data/board.eps", "r");
    if (template_in == NULL) {
        handle_error("Cannot read PostScript board template file");
    }

    int buf[EPS_BUFFER_SIZE];
    size_t r;
    while ((r = fread(buf, 1, EPS_BUFFER_SIZE, template_in)) > 0) {
        fwrite(buf, 1, r, eps_out);
    }
    fclose(template_in);
    fputs("\n% board data\n", eps_out);
    for (int c = 0; c < NUM_COLS; c++) {
        for (int r = 0; r < NUM_ROWS; r++) {
            if (board_get(b, r, c) == WHITE) {
                fprintf(eps_out, "%d %d putwhite\n", r, c);
            } else if (board_get(b, r, c) == BLACK) {
                fprintf(eps_out, "%d %d putblack\n", r, c);
            }
        }
    }

    if  (b->turn == WHITE) {
        fputs("whitetomove\n", eps_out);
    } else {
        fputs("blacktomove\n", eps_out);
    }

    if (b->winner == WHITE) {
        fputs("whitewins\n", eps_out);
    } else if (b->winner == BLACK) {
        fputs("blackwins\n", eps_out);
    } else if (board_full(b)) {
        fputs("draw\n", eps_out);
    }
}
