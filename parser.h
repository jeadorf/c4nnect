#ifndef _PARSER_H
#define	_PARSER_H

#include <stdio.h>
#include "board.h"

/* Takes a string representation of a board and compiles it into a Board struct
 * object.
 */
void parser_read(Board *b, char *data);

/* Reads a string representation of a board from a stream and compiles it into
 * a Board struct object. */
void parser_fread(Board *b, FILE *stream);

#endif	/* _PARSER_H */

