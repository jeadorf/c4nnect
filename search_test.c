#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "minunit.h"
#include "parser.h"
#include "search.h"
#include "search_test.h"

static char* test_abn_white_win() {
    printf("test_abn_white_win\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- b b - b - -"
            "- w w - w - -");
    Move res;
    alphabeta_negamax(&b, WHITE, -FLT_MAX, FLT_MAX, 0, 2, &res);
    mu_assert("error, minimax value should be very big", res.value > 90);
    mu_assert("error, should find winning move", res.column == 3);
    return 0;
}

static char* test_abn_white_win2() {
    printf("test_abn_white_win2\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "b - - - - - -"
            "w - - b - - -"
            "b - - w w - -"
            "b - - w w - -"
            "b - b w w w b");
    Move res;
    alphabeta_negamax(&b, WHITE, -FLT_MAX, FLT_MAX, 0, 1, &res);
    mu_assert("error, should find winning move", res.column == 4);
    mu_assert("error, minimax value should be very big", res.value > 90);
    return 0;
}

static char* test_abn_black_win() {
    printf("test_abn_black_win\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- w w - w - -"
            "- b b - b - -");
    Move res;
    alphabeta_negamax(&b, BLACK, -FLT_MAX, FLT_MAX, 0, 1, &res);
    mu_assert("error, should find winning move", res.column == 3);
    mu_assert("error, minimax value should be very big", res.value > 90);
    return 0;
}

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
    mu_assert("error, should find winning move", search(&b, WHITE) == 3);
    // This is dangerous to test. There might be no way for black to avoid
    // defeat in this position though this is rather unlikely.
    mu_assert("error, should find saving move", search(&b, BLACK) == 3);
    return 0;
}

static char* test_search_white_win2() {
    printf("test_search_white_win2\n");
    Board b;
    // Though not strictly necessary, the engine should find not only the move
    // that leads to a victory but also the fastest one. In this case, putting a
    // piece in column 2 or 5 would be sufficient to win since it creates
    // another threat and black cannot avoid to lose anyway. Yet, there is a
    // faster way to victory by simply putting a piece into column 4 and
    // completing the tower to four pieces.
    parser_read(&b,
            "- - - - - - -"
            "b - - - - - -"
            "w - - b - - -"
            "b - - w w - -"
            "b - - w w - -"
            "b - b w w w b");

    mu_assert("error, should find fastest winning move", search(&b, WHITE) == 4);
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
    mu_assert("error, should find winning move", search(&b, BLACK) == 3);
    mu_assert("error, should find saving move", search(&b, WHITE) == 3);
    return 0;
}

static char* test_search_white_win3() {
    printf("test_search_white_win3\n");
    Board b;
    parser_read(&b,
            "b - w w - - -"
            "b - w w w - -"
            "w b w b b - -"
            "b b b w w b -"
            "b w b w w w -"
            "b b b w w b -");
    mu_assert("error, should find winning move h", search(&b, WHITE) == 1);
    return 0;
}

static char* test_search_defer_defeat() {
    printf("test_search_defer_defeat\n");
    Board b;
    // This position is obviously lost for black, even if it is his turn because
    // white can either complete the tower in column 4 or initiate a double
    // threat in column 5. Yet, any optimistic black player would at least
    // respond to the obvious threat of the tower on 4, therefore deferring
    // his defeat (which is only sure to happen if white plays correctly) at
    // least for two plies.
    parser_read(&b,
            "- - - - - - -"
            "b - - - - - -"
            "w - - b - - -"
            "b - - w w - -"
            "b - - w w - -"
            "b - b w w w b");
    mu_assert("error, should defer defeat", search(&b, BLACK) == 4);
    return 0;
}

static char* all_tests() {
    mu_run_test(test_abn_white_win);
    mu_run_test(test_abn_black_win);
    mu_run_test(test_abn_white_win2);
    mu_run_test(test_search_white_win);
    mu_run_test(test_search_white_win2);
    mu_run_test(test_search_white_win3);
    mu_run_test(test_search_black_win);
    mu_run_test(test_search_defer_defeat);
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
