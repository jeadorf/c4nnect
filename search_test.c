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

static char* assert_surjectivity(int8_t *moves) {
    for (int k = 0; k < NUM_COLS; k++) {
        bool found = false;
        for (int l = 0; l < NUM_COLS; l++) {
            if (moves[l] == k) {
                found = true;
            }
        }
        mu_assert("Not all moves have been generated!", found);
    }
    return 0;
}

static char* test_genmove_surjectivity_simple() {
    printf("test_genmove_surjectivity_simple\n");
    Board b;
    board_init(&b);
    Variation var;
    variation_init(&var);
    int8_t moves[NUM_COLS];
    generate_moves(&var, 0, moves);
    assert_surjectivity(moves);
    return 0;
}

static char* test_genmove_pv_first() {
    printf("test_genmove_pv_first\n");
    Board b;
    board_init(&b);
    Variation var;
    variation_init(&var);
    var.length = 1;
    for (int8_t col = 0; col < NUM_COLS; col++) {
        var.moves[0] = col;
        int8_t moves[NUM_COLS];
        generate_moves(&var, 0, moves);
        assert_surjectivity(moves);
        mu_assert("Expected move from principal variation to be first", moves[0] == var.moves[0]);
    }
    return 0;
}

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
    fboard2eps(&b, "build/test_abn_white_win.eps");
    Variation var;
    variation_init(&var);
    SearchRecord rec;
    searchrecord_init(&rec);
    alphabeta_negamax(&b, ALPHA_MIN, BETA_MAX, 0, 2, true, false, &var, &rec);
    mu_assert("error, minimax value should be very big", var.rating > 90);
    mu_assert("error, should find winning move", var.moves[0] == 3);

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
    Variation var;
    variation_init(&var);
    SearchRecord rec;
    searchrecord_init(&rec);
    alphabeta_negamax(&b, ALPHA_MIN, BETA_MAX, 0, 1, true, false, &var, &rec);
    mu_assert("error, should find winning move", var.moves[0] == 4);
    mu_assert("error, minimax value should be very big", var.rating > 90);

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
    Variation var;
    variation_init(&var);
    SearchRecord rec;
    searchrecord_init(&rec);
    alphabeta_negamax(&b, ALPHA_MIN, BETA_MAX, 0, 1, true, false, &var, &rec);
    mu_assert("error, should find winning move", var.moves[0] == 3);
    mu_assert("error, minimax value should be very big", var.rating > 90);

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

 __attribute__ ((unused)) static char* test_search_fastest_white_win() {
    printf("test_search_fastest_white_win\n");
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
    fboard2eps(&b, "build/test_search_fastest_white_win.eps");
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

 __attribute__ ((unused)) static char* test_fast_black_win() {
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

static char* test_white_difficult_win() {
    printf("test_white_difficult_win\n");
    Board b;
    parser_read(&b,
            "- - - b b - - "
            "- - w w b w - "
            "- - w w w b - "
            "- - b b w b - "
            "- - w w b b - "
            "- - b w w w b ");
    fboard2eps(&b, "build/test_white_difficult_win.eps");
    Variation var;
    variation_init(&var);
    SearchRecord rec;
    searchrecord_init(&rec);
    mu_assert("It should be black's turn", b.turn == BLACK);
    search(&b, &var, &rec);
    mu_assert("Position should be classified as a white win", var.rating <= ALPHA_MIN);
    while (b.winner == NOBODY && !board_full(&b)) {
        board_put(&b, searchm(&b));
    }
    mu_assert("Game should be won by white", b.winner == WHITE);

    return 0;
}

static char* test_time_consuming_position() {
    printf("test_time_consuming_position\n");
    Board b;
    // For some reason, this position requires a lot of search time (at least if
    // the transposition tables are empty. All other position seem to be solved
    // within at most a second, some deviate to up to 4 seconds (very rare) but
    // this position needs more than a minute! This is a position where the
    // search reaches a depth of 20 plies, which explains a lot. Eventually,
    // white should find a winning strategy. And probably, it is the defeat
    // deferral making things even worse with transposition tables being disabled
    // TODO: an idea - limit search depth of defeat deferral
    parser_read(&b,
            "- - - - - - - "
            "w - - - - - - "
            "w - - - b - - "
            "b - - - w w - "
            "w w w b b w - "
            "b b w b b b w ");
    fboard2eps(&b, "build/test_time_consuming_position.eps");
    mu_assert("Should be black's turn", b.turn == BLACK);
    searchm(&b);
    return 0;
}

static char* test_time_consuming_position2() {
    printf("test_time_consuming_position2\n");
    Board b;
    // This position repeatedly shows a very huge gap between two steps of the iterative
    // search, giving the following trace, something that cannot be guessed by
    // the time estimator.
    //
    // estimated time 0ms, actual time: 0ms
    // estimated time 0ms, actual time: 0ms
    // estimated time 0ms, actual time: 0ms
    // estimated time 0ms, actual time: 0ms
    // estimated time 0ms, actual time: 10ms
    // estimated time 0ms, actual time: 10ms
    // estimated time 0ms, actual time: 10ms
    // estimated time 0ms, actual time: 20ms
    // estimated time 40ms, actual time: 30ms
    // estimated time 45ms, actual time: 30ms
    // estimated time 30ms, actual time: 40ms
    // estimated time 53ms, actual time: 60ms
    // estimated time 90ms, actual time: 80ms
    // estimated time 106ms, actual time: 140ms
    // estimated time 245ms, actual time: 200ms
    // estimated time 285ms, actual time: 123420ms
    // NOTE: this problem does no longer occur, leaving this test in order to
    // detect possible regression.
    parser_read(&b,
            "- - - - - - - "
            "- - - - - - - "
            "- - - - - - - "
            "- - - - - - w "
            "- - w - b w w "
            "b - w b b b w ");
    fboard2eps(&b, "build/test_time_consuming_position2.eps");
    Variation var;
    variation_init(&var);
    SearchRecord rec;
    searchrecord_init(&rec);
    search(&b, &var, &rec);
    // check whether this problem might be caused by defeat deferral re-search
    mu_assert("might be defeat deferral causing the time consumption", rec.defeat_deferred == false);
    return 0;
}

static char* test_time_consuming_position3() {
    printf("test_time_consuming_position3\n");
    Board b;
    parser_read(&b,
            "- - - - - - - "
            "- - w - - - - "
            "w - w - b - b "
            "w - b - w - b "
            "b w w b b - w "
            "w w b b w b b ");
    fboard2eps(&b, "build/test_time_consuming_position3.eps");
    board_print(&b);
    Variation var;
    variation_init(&var);
    SearchRecord rec;
    searchrecord_init(&rec);
    search(&b, &var, &rec);
    mu_assert("search took much too much time", (rec.cpu_time / (CLOCKS_PER_SEC / 1000)) < 10 * TIME_LIMIT_PER_PLY);
    return 0;
}

static char* all_tests() {
    mu_run_test(test_genmove_surjectivity_simple);
    mu_run_test(test_genmove_pv_first);
    mu_run_test(test_abn_white_win);
    mu_run_test(test_abn_black_win);
    mu_run_test(test_abn_white_win2);
    mu_run_test(test_search_white_win);
    /*
        mu_run_test(test_search_fastest_white_win);
     */
    mu_run_test(test_search_white_win3);
    mu_run_test(test_search_black_win);
    mu_run_test(test_beginning_trap_white);
    mu_run_test(test_beginning_trap_black);
    mu_run_test(test_search_defer_defeat);
    /*
        mu_run_test(test_fast_black_win);
     */
    mu_run_test(test_white_difficult_win);
    mu_run_test(test_time_consuming_position);
    mu_run_test(test_time_consuming_position2);
    mu_run_test(test_time_consuming_position3);

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

