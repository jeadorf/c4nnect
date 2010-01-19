#ifndef _BOARD2EPS_H
#define	_BOARD2EPS_H

#include <stdio.h>
#include "board.h"

void board2eps(Board *b, FILE *eps_out);
void fboard2eps(Board *b, char *filename);

#endif	/* _BOARD2EPS_H */

