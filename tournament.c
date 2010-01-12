#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "util.h"
#include "board.h"

// TODO: make address configurable
#define TOURNAMENT_SOCKET_ADDRESS "/tmp/fiart"

/* Starts a tournament match connecting two players. Goals:
 * - one or both of the players might be human
 * - one or both of the players might be computers
 * - there is a time limit for the whole game
 * - each player can invest as much time as he likes to as long as not exceeding
 *   his overall time limit
 * - the communication protocol should be rather easy to implement
 * - players should be able to ponder when it's their opponent's turn
 */
void tournament_start() {
    FILE *in_one;
    FILE *in_two;

    int pid = getpid();
    // See http://www.cs.cf.ac.uk/Dave/C
    int s;
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        // TODO: error handling
        handle_error("Cannot create tournament socket");
    }

    // Construct socket address
    // sun probably stands for Socket UNix and not for the
    // company.
    struct sockaddr_un saddr;
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path, TOURNAMENT_SOCKET_ADDRESS);

    // Bind socket
    unlink(TOURNAMENT_SOCKET_ADDRESS);
    int len = sizeof (saddr.sun_family) + strlen(saddr.sun_path);
    if (bind(s, &saddr, len) < 0) {
        // TODO: error handling
        handle_error("Could not bind tournament server");
    }

    // TODO: Only allow two pending connections in the queue
    if (listen(s, 5) < 0) {
        // TODO: error handling
        handle_error("Tournament server could not start listening");
    }

    // Connect to both players
    struct sockaddr_un addr_one;
    struct sockaddr_un addr_two;
    int fd_one, fd_two, len_one, len_two;
    fd_one = accept(s, &addr_one, &len_one);
    if (fd_one < 0) {
        // TODO: error handling
        handle_error("Tournament server could not accept connection to player 1");
    }

    // TODO: Only allow two pending connections in the queue
    if (listen(s, 5) < 0) {
        // TODO: error handling
        handle_error("Tournament server could not start listening");
    }
    len_two = sizeof (saddr.sun_family) + strlen(saddr.sun_path);
    fd_two = accept(s, &addr_two, &len_two);
    if (fd_two < 0) {
        // TODO: error handling
        handle_error("Tournament server could not accept connection to player 2");
    }

    in_one = fdopen(fd_one, "r");
    in_two = fdopen(fd_two, "r");
 
    char buf[64];
    int col;
    Board b;
    board_init(&b);
    // give player one a hint that he begins
    write_data(fd_one, "start!");
    while (true) {
        // Waiting for move from player one
        readline(in_one, buf, 64);
        col = atoi(buf);
        // TODO: check validity
        board_put(&b, WHITE, col);
        board_print(&b);
        if (b.winner != NOBODY || board_full(&b)) {
            break;
        }
        write_data(fd_two, buf);
        readline(in_two, buf, 64);
        col = atoi(buf);
        // TODO: check validity
        board_put(&b, BLACK, col);
        board_print(&b);
        if (b.winner != NOBODY || board_full(&b)) {
            break;
        }
        write_data(fd_one, buf);
    }
    printf("col %d\n", col);

    close(s);
    unlink(TOURNAMENT_SOCKET_ADDRESS);
}

void tournament_client() {
    int s;
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        // TODO: error handling
        handle_error("Tournament client could not create socket");
    }

    // Server address
    struct sockaddr_un saddr;
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path, TOURNAMENT_SOCKET_ADDRESS);
    int len = sizeof (saddr.sun_family) + strlen(saddr.sun_path);
    int err;
    if (err = connect(s, &saddr, len) < 0) {
        // TODO: error handling
        handle_error("Tournament client could not connect to server");
    }

    FILE *cltin;
    cltin = fdopen(s, "r");

    // FIXME: not done yet
    char buf[64];
    Board b;
    board_init(&b);
    while (true) {
        readline(cltin, buf, 64);
        write_data(s, buf);
    }

    close(s);
}

void tournament_self() {
    if (fork() == 0) {
        // TODO: synchronize processes correctly
        sleep(3);
        tournament_client();
        exit(EXIT_SUCCESS);
    }
    if (fork() == 0) {
        // TODO: synchronize processes correctly
        sleep(3);
        tournament_client();
        exit(EXIT_SUCCESS);
    }

    tournament_start();
}
