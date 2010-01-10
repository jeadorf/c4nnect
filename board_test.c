#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include "minunit.h"
#include "board.h"
#include "parser.h"

int tests_run = 0;

static char* test_board_put() {
    printf("test_board_put\n");
    Board b;
    board_init(&b);
    board_put(&b, BLACK, 3);
    mu_assert("error, could not place white piece", board_get(&b, 0, 3) == BLACK);
    mu_assert("error, slot should be empty", board_get(&b, 0, 0) == NOBODY);
    return 0;
}

static char* test_board_full() {
    printf("test_board_full\n");
    int r;
    Board b;
    board_init(&b);
    for (r = 0; r < NUM_ROWS; r++) {
        mu_assert("error, column cannot be full", !board_full(&b, 3));
        board_put(&b, BLACK, 3);
    }
    mu_assert("error, column must be full", board_full(&b, 3));
    return 0;
}

static char* test_board_move_wins_col() {
    printf("test_board_move_wins_col\n");
    Board b;
    board_init(&b);
    for (int i = 0; i < 4; i++) {
        mu_assert("error, game is still drawn", b.winner == NOBODY);
        board_put(&b, BLACK, 3);
    }
    mu_assert("error, game is won", b.winner == BLACK);
    return 0;
}

static char* test_board_move_wins_row() {
    printf("test_board_move_wins_row\n");
    Board b;
    board_init(&b);
    for (int i = 1; i < 5; i++) {
        mu_assert("error, game is still drawn", b.winner == NOBODY);
        board_put(&b, BLACK, i);
    }
    mu_assert("error, game is won", b.winner == BLACK);
    return 0;
}

static char* test_board_move_wins_row2() {
    printf("test_board_move_wins_row2\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- b - - - - -"
            "w w - - - - -"
            "w w - - - - -"
            "w b b - b b -");
    mu_assert("error, game is still drawn", b.winner == NOBODY);
    board_put(&b, BLACK, 3);
    mu_assert("error, black should have won", b.winner == BLACK);
    return 0;
}

static char* test_board_move_wins_diagup() {
    printf("test_board_move_wins_diagup\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - w -"
            "- - - - w b -"
            "- - - w b b -"
            "- - w b w b -");
    mu_assert("error, white should have won", b.winner == WHITE);
    return 0;
}

static char* test_board_move_wins_diagup2() {
    printf("test_board_move_wins_diagup2\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - b -"
            "- - - - b w -"
            "- - - b w w -"
            "- - b w b w -");
    mu_assert("error, black should have won", b.winner == BLACK);
    return 0;
}

static char* test_board_move_wins_diagdown() {
    printf("test_board_move_wins_diagdown\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - w - - - -"
            "- - b w - - -"
            "- - b b w - -"
            "- - b w b w -");
    mu_assert("error, white should have won", b.winner == WHITE);
    return 0;
}

static char* test_board_move_wins_diagdown2() {
    printf("test_board_move_wins_diagdown2\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - b - - - -"
            "- - w b - - -"
            "- - w w b - -"
            "- - w b w b -");
    mu_assert("error, black should have won", b.winner == BLACK);
    return 0;
}

static char* all_tests() {
    mu_run_test(test_board_put);
    mu_run_test(test_board_full);
    mu_run_test(test_board_move_wins_col);
    mu_run_test(test_board_move_wins_row);
    mu_run_test(test_board_move_wins_row2);
    mu_run_test(test_board_move_wins_diagup);
    mu_run_test(test_board_move_wins_diagup2);
    mu_run_test(test_board_move_wins_diagdown);
    mu_run_test(test_board_move_wins_diagdown2);
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
