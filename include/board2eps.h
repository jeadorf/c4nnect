#ifndef _BOARD2EPS_H
#define _BOARD2EPS_H

#include <stdio.h>
#include "board.h"

/*! Exports a position to encapsulated PostScript. */
void board2eps(Board *b, FILE *eps_out);

/*! Exports a position to an encapsulated PostScript file. */
void fboard2eps(Board *b, char *filename);

#endif	/* _BOARD2EPS_H */

