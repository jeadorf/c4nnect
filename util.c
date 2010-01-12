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

void readline(FILE* stream, char *buf, int maxlen) {
    char c;
    int i = 0;
    while (i < maxlen - 1 && (c = fgetc(stream)) != EOF) {
        buf[i] = c;
        i++;
        if (c == '\n' || c == '\0') {
            break;
        }
    }
    buf[i] = '\0';
}

void write_data(FILE* stream, char *msg) {
    write(stream, msg, strlen(msg));
    write(stream, "\n", 1);
}
