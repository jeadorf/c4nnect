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
    for (int i = 0; i < 64; i++) {
        if (i != 0 && (i % 9 == 0 || i % 9 == 6)) {
            putchar('-');
        }
        putchar((n & (1UL << i)) ? '1' : '0');
    };
    printf(" --- 0x%.16lX\n", n);

}
