#ifndef _PARSER_H
#define	_PARSER_H

#include <stdio.h>
#include "board.h"

/*! Takes a string representation of a board and constructs a Board struct
 * object from it. */
void parser_read(Board *b, char *data);

void parser_fread(Board *b, FILE *stream);

#endif	/* _PARSER_H */

