#ifndef _UTIL_H
#define	_UTIL_H

#include <stdio.h>

void handle_error(char *msg);

void write_data(FILE* stream, char *msg);

void readline(FILE* stream, char *buf, int maxlen);

#endif	/* _UTIL_H */

