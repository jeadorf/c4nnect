#include <malloc.h>
#include <stdio.h>
#include "minunit.h"
#include "board.h"

int tests_run = 0;

static char* test_board_put() {
    Board *b = malloc(sizeof (Board));
    board_put(b, BLACK, 3);
    board_print(b);
    mu_assert("error, could not place white piece", board_get(b, 0, 3) == BLACK);
    mu_assert("error, slot should be empty", board_get(b, 0, 0) == NOBODY);
    return 0;
}

static char* test_board_full() {
    int r;
    Board *b = malloc(sizeof (Board));
    for (r = 0; r < NUM_ROWS; r++) {
        mu_assert("error, column cannot be full", !board_full(b, 3));
        board_put(b, BLACK, 3);
    }
    mu_assert("error, column must be full", board_full(b, 3));
}

static char* all_tests() {
    mu_run_test(test_board_put);
    mu_run_test(test_board_full);
    return 0;
}

int board_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
