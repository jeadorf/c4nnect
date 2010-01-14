#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "minunit.h"
#include "parser.h"
#include "stats.h"
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
    Statistics stats;
    stats_init(&stats);
    alphabeta_negamax(&b, WHITE, -FLT_MAX, FLT_MAX, 0, 2, &res, &stats);
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
    Statistics stats;
    stats_init(&stats);
    alphabeta_negamax(&b, WHITE, -FLT_MAX, FLT_MAX, 0, 1, &res, &stats);
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
    Statistics stats;
    stats_init(&stats);
    alphabeta_negamax(&b, BLACK, -FLT_MAX, FLT_MAX, 0, 1, &res, &stats);
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

static char* test_beginning_trap_white() {
    printf("test_beginning_trap_white\n");
    Board b;
    // This board is an example for a situation when a two-ply lookahead is
    // insufficient to find out that black will lose when he does not counter the
    // threat by putting a piece into column 1 or 4. A deep search will surely
    // find out. But the "deferral technique" used in an earlier version turned
    // out to be incorrect.
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - b - - -"
            "- - w w - - -");
    int col = search(&b, BLACK);
    mu_assert("error, should avoid trap in the beginning", col == 1 || col == 4);
    return 0;
}

static char* test_beginning_trap_black() {
    printf("test_beginning_trap_black\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - w - - -"
            "- - b b - - -");
    int col = search(&b, WHITE);
    mu_assert("error, should avoid trap in the beginning", col == 1 || col == 4);
    return 0;
}

static char* test_fast_black_win() {
    printf("test_fast_black_win\n");
    Board b;
    parser_read(&b,
            "- - - b w - -"
            "- - - b w - -"
            "- - - w w - -"
            "- - b b b - -"
            "- - w b b - -"
            "w w b b w w w");

    int col = search(&b, BLACK);
    mu_assert("error, should choose 3-ply win", col == 5);
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

static char* test_maximum_search_depth() {
    printf("test_maximum_search_depth\n");
    // The idea of this test is to determine how deep we can search without
    // wasting too much time.
    Board b;
    parser_read(&b,
            "w - - - - - -"
            "w - - - - - -"
            "b - - - - - -"
            "w - b b - - -"
            "w - b b - - -"
            "w b w w b - -");
    Statistics stats;
    stats_init(&stats);
    Move mv;
    alphabeta_negamax(&b, WHITE, -FLT_MAX, FLT_MAX, 0, 42, &mv, &stats);
    
    Board b2;
    parser_read(&b2,
            "- - - - - - -"
            "b - - - - - -"
            "b - - - - - -"
            "w - b b - - -"
            "w - b b w w b"
            "w b w w b b w");
    Statistics stats2;
    stats_init(&stats2);
    Move mv2;
    alphabeta_negamax(&b2, WHITE, -FLT_MAX, FLT_MAX, 0, 42, &mv2, &stats2);
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
    mu_run_test(test_beginning_trap_white);
    mu_run_test(test_beginning_trap_black);
    mu_run_test(test_search_defer_defeat);
    mu_run_test(test_fast_black_win);
    mu_run_test(test_maximum_search_depth);
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
