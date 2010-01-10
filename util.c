#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void handle_error(char *msg) {
    fputs("ERROR: ", stderr);
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(EXIT_FAILURE);
}
