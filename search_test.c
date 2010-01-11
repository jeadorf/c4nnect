#include <stdio.h>
#include "board.h"
#include "minunit.h"
#include "parser.h"
#include "search.h"
#include "search_test.h"

static char* test_search_white_win() {
    printf("test_search_white_win\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- b b - b - -"
            "- w w - w - -");
    mu_assert("error, should find winning move", search(b, WHITE) == 3);
    mu_assert("error, should find saving move", search(b, BLACK) == 3);
    return 0;
}

static char* test_search_white_win2() {
    printf("test_search_white_win2\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "b - - - - - -"
            "w - - b - - -"
            "b - - w w - -"
            "b - - w w - -"
            "b - b w w w b");
    printf("move: %d\n", search(b, WHITE));
    mu_assert("error, should find winning move", search(b, WHITE) == 4);
    mu_assert("error, should find saving move", search(b, BLACK) == 4);
    return 0;
}

static char* test_search_black_win() {
    printf("test_search_black_win\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- w w - w - -"
            "- b b - b w -");
    mu_assert("error, should find winning move", search(b, BLACK) == 3);
    mu_assert("error, should find saving move", search(b, WHITE) == 3);
    return 0;
}

static char* all_tests() {
    mu_run_test(test_search_white_win);
    mu_run_test(test_search_white_win2);
    mu_run_test(test_search_black_win);
    return 0;
}

int search_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
