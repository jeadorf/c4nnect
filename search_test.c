#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "eval.h"
#include "minunit.h"
#include "parser.h"
#include "stats.h"
#include "search.h"
#include "search_test.h"
#include "board2eps.h"

static char* test_abn_white_win() {
    printf("build/test_abn_white_win\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- b b - b - -"
            "- w w - w - -");
    fboard2eps(&b, "build/test_abn_white_win.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    alphabeta_negamax(&b, ALPHA_MIN, BETA_MAX, 0, 2, &rec);
    mu_assert("error, minimax value should be very big", rec.rating  > 90);
    mu_assert("error, should find winning move", rec.move == 3);
    return 0;
}

static char* test_abn_white_win2() {
    printf("test_abn_white_win2\n");
    Board b;
    parser_read(&b,
            "b - - - - - -"
            "b - - - - - -"
            "w - - b - - -"
            "b - - w w - -"
            "b - - w w - -"
            "b - b w w w b");
    fboard2eps(&b, "build/test_abn_white_win2.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    alphabeta_negamax(&b, ALPHA_MIN, BETA_MAX, 0, 1, &rec);
    mu_assert("error, should find winning move", rec.move == 4);
    mu_assert("error, minimax value should be very big", rec.rating > 90);
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
            "w w - - w - -"
            "w b b - b - -");
    fboard2eps(&b, "build/test_abn_black_win.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    alphabeta_negamax(&b, ALPHA_MIN, BETA_MAX, 0, 1, &rec);
    mu_assert("error, should find winning move", rec.move == 3);
    mu_assert("error, minimax value should be very big", rec.rating > 90);
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
    fboard2eps(&b, "build/test_search_white_win.eps");
    mu_assert("error, should find winning move", searchm(&b) == 3);
    // This is dangerous to test. There might be no way for black to avoid
    // defeat in this position though this is rather unlikely.
    board_put(&b, 1); // add white piece, let black move
    mu_assert("error, should find saving move", searchm(&b) == 3);
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
    fboard2eps(&b, "build/test_search_white_win2.eps");
    mu_assert("error, should find fastest winning move", searchm(&b) == 4);
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
    fboard2eps(&b, "build/test_search_black_win.eps");
    mu_assert("error, should find winning move", searchm(&b) == 3);
    mu_assert("error, should find saving move", searchm(&b) == 3);
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
    fboard2eps(&b, "build/test_search_white_win3.eps");
    mu_assert("error, should find winning move h", searchm(&b) == 1);
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
    fboard2eps(&b, "build/test_beginning_trap_white.eps");
    int8_t col = searchm(&b);
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
            "- - w w - - -"
            "- - b b - - -");
    fboard2eps(&b, "build/test_beginning_trap_black.eps");
    int8_t col = searchm(&b);
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
    fboard2eps(&b, "build/test_fast_black_win.eps");
    mu_assert("error, should choose 3-ply win", 5 == searchm(&b));
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
    fboard2eps(&b, "build/test_search_defer_defeat.eps");
    mu_assert("error, should defer defeat", searchm(&b) == 4);
    return 0;
}

// TODO: need me or delete me
static char* test_maximum_search_depth() {
    printf("test_maximum_search_depth\n");
    // The idea of this test is to determine how deep we can search without
    // wasting too much time. It's not in the standard set of tests
    Board b;
    parser_read(&b,
            "w - - - - - -"
            "w - - - - - -"
            "b - - - - - -"
            "w - b b - - -"
            "w - b b - - -"
            "w b w w b - -");
    fboard2eps(&b, "build/test_maximum_search_depth-1.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    alphabeta_negamax(&b, ALPHA_MIN, BETA_MAX, 0, 42, &rec);
    
    Board b2;
    parser_read(&b2,
            "- - - - - - -"
            "b - - - - - -"
            "b - - - - - -"
            "w - b b - - -"
            "w - b b w w b"
            "w b w w b b w");
    fboard2eps(&b, "build/test_maximum_search_depth-2.eps");
    SearchRecord rec2;
    searchrecord_init(&rec2);
    alphabeta_negamax(&b2, ALPHA_MIN, BETA_MAX, 0, 42, &rec2);
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
    return 0;
}

int search_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS RUN\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}

