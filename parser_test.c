#include <stdio.h>
#include "board.h"
#include "minunit.h"
#include "parser.h"
#include "parser_test.h"
#include "board2eps.h"

static char* test_parse_empty() {
    printf("test_parse_empty\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -");
    fboard2eps(&b, "build/test_parse_empty.eps");
    for (int8_t i = 0; i < NUM_COLS; i++) {
        mu_assert("error, column expected to be empty", board_get_top(&b, i) == NOBODY);
    }

    return 0;
}

static char* test_parse_one() {
    printf("test_parse_one\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - w - - -");
    fboard2eps(&b, "build/test_parse_one.eps");
    for (int8_t i = 0; i < NUM_COLS; i++) {
        if (i != 3) {
            mu_assert("error, column expected to be empty", board_get_top(&b, i) == NOBODY);
        }
    }
    mu_assert("error, expected white piece", board_get_top(&b, 3) == WHITE);

    return 0;
}

static char* test_parse_quoted() {
    printf("test_parse_quoted\n");
    Board b;
    parser_read(&b,
            "\"- - - - - - -\""
            "\"- - - - - - -\""
            "\"- - - - - - -\""
            "\"- - - - - - -\""
            "\"- - - - - - -\""
            "\"w - - b - - -\"");
    fboard2eps(&b, "build/test_parse_quoted.eps");
    for (int8_t i = 1; i < NUM_COLS; i++) {
        if (i != 3) {
            mu_assert("error, column expected to be empty", board_get_top(&b, i) == NOBODY);
        }
    }
    mu_assert("error, expected black piece", board_get_top(&b, 3) == BLACK);
    mu_assert("error, expected black piece", board_get_top(&b, 0) == WHITE);

    return 0;
}

static char* test_white_to_move() {
    printf("test_white_to_move\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "b - - - - - -"
            "w w - - w - -"
            "w b b - b - -");
    fboard2eps(&b, "build/test_white_to_move.eps");
    mu_assert("error, it should be white's turn", b.turn == WHITE);
    return 0;
}

static char* test_black_to_move() {
    printf("test_black_to_move\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "w w - - w - -"
            "w b b - b - -");
    fboard2eps(&b, "build/test_black_to_move.eps");
    mu_assert("error, it should be black's turn", b.turn == BLACK);
    return 0;
}

static char* all_tests() {
    mu_run_test(test_parse_empty);
    mu_run_test(test_parse_one);
    mu_run_test(test_parse_quoted);
    mu_run_test(test_white_to_move);
    mu_run_test(test_black_to_move);
    return 0;
}

int parser_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS RUN\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
