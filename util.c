#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

void handle_error(char *msg) {
    fputs("ERROR: ", stderr);
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(EXIT_FAILURE);
}

void print_unsigned_long_rev(unsigned long n) {
    for (int i = 0; i < 63; i++) {
        putchar((n & (1UL << i)) ? '1' : '0');
    }
    putchar('\n');

}
