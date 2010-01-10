#ifndef _PARSER_H
#define	_PARSER_H

#include "board.h"

/* Takes a string representation of a board and compiles it into a Board struct
 * object.
 */
void parser_read(Board *b, char *data);

#endif	/* _PARSER_H */

