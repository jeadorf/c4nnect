#include <stdio.h>
#include "board.h"
#include "minunit.h"
#include "parser.h"
#include "parser_test.h"

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
            "- - - b - - -");
    for (int8_t i = 0; i < NUM_COLS; i++) {
        if (i != 3) {
            mu_assert("error, column expected to be empty", board_get_top(&b, i) == NOBODY);
        }
    }
    mu_assert("error, expected black piece", board_get_top(&b, 3) == BLACK);

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
            "\"- - - b - - -\"");
    for (int8_t i = 0; i < NUM_COLS; i++) {
        if (i != 3) {
            mu_assert("error, column expected to be empty", board_get_top(&b, i) == NOBODY);
        }
    }
    mu_assert("error, expected black piece", board_get_top(&b, 3) == BLACK);

    return 0;
}


static char* all_tests() {
    mu_run_test(test_parse_empty);
    mu_run_test(test_parse_one);
    mu_run_test(test_parse_quoted);
    return 0;
}

int parser_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
