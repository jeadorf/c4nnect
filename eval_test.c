#include <stdio.h>
#include <math.h>
#include "eval.h"
#include "eval_test.h"
#include "board.h"
#include "search.h"
#include "stats.h"
#include "minunit.h"
#include "parser.h"
#include "board2eps.h"

static char* test_eval_white_win() {
    printf("test_eval_white_win\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - b - b - -"
            "- w w w w b -");
    fboard2eps(&b, "build/test_eval_white_win.eps");
    mu_assert("error, evaluation value should be positive", eval(&b, false) >= 0);
    mu_assert("error, evaluation value should be big", eval(&b, false) >= BETA_MAX);
    return 0;
}

static char* test_eval_black_win() {
    printf("test_eval_black_win\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - w w w - -"
            "- w b b b b -");
    fboard2eps(&b, "build/test_eval_black_win.eps");
    mu_assert("error, evaluation value should be negative", eval(&b, false) < 0);
    mu_assert("error, evaluation value should be negative "
              "with big absolute value", eval(&b, false) <= ALPHA_MIN);
    return 0;
}

static char* test_odd_even_white_win() {
    printf("test_odd_even_white_win\n");
    Board b;
    // White to move, white will win
    parser_read(&b,
            "w - b b - b b"
            "b - w b - b w"
            "w - w b - w b"
            "b b w w b w b"
            "w w b w w b w"
            "b w w w b b w");
    fboard2eps(&b, "build/test_odd_even_white_win.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    search(&b, &rec);
    mu_assert("error, expected white to move (tester's note)", b.turn == WHITE);
    mu_assert("error, expected white to win", rec.rating >= BETA_MAX);
    mu_assert("error, this position should be classified as an advantage for white", eval_odd_even(&b) > 0);
    return 0;
}

static char* test_odd_even_white_win2() {
    printf("test_odd_even_white_win2\n");
    Board b;
    // Black to move, white will win, see test_odd_even_white_win
    parser_read(&b,
            "w - b b - b b"
            "b - w b - b w"
            "w - w b w w b"
            "b b w w b w b"
            "w w b w w b w"
            "b w w w b b w");
    fboard2eps(&b, "build/test_odd_even_white_win2.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    search(&b, &rec);
    mu_assert("error, expected black to move (tester's note)", b.turn == BLACK);
    mu_assert("error, expected white to win", rec.rating <= ALPHA_MIN);
    mu_assert("error, this position should be classified as an advantage for white", eval_odd_even(&b) > 0);
    return 0;
}

static char* test_odd_even_white_win3() {
    printf("test_odd_even_white_win3\n");
    Board b;
    // Taken from: A Knowledge-based Approach of Connect-Four
    // Black to move, white will win
    parser_read(&b,
            "w - b w - b b"
            "b - w b - w w"
            "w - b w - w b"
            "b - w b - w b"
            "w - w b - b w"
            "b w b w - b w");
    fboard2eps(&b, "build/test_odd_even_white_win3.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    search(&b, &rec);
    mu_assert("error, expected black to move (tester's note)", b.turn == BLACK);
    mu_assert("error, expected white to win", rec.rating <= ALPHA_MIN);
    mu_assert("error, this position should be classified as an advantage for white", eval_odd_even(&b) > 0);
    return 0;
}

static char* test_odd_even_draw() {
    printf("test_odd_even_draw\n");
    Board b;
    parser_read(&b,
            "w - b w - b b"
            "b - w b - w w"
            "w - w b - w b"
            "b - w b - w b"
            "b w b w w b b"
            "b w w w b b w");
    fboard2eps(&b, "build/test_odd_even_draw.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    search(&b, &rec);
    mu_assert("error, expected white to move (tester's note)", b.turn == WHITE);
    mu_assert("error, expected game to be drawn", rec.rating == 0); // TODO: comparison with zero
    mu_assert("error, this position should be classified as drawn", eval_odd_even(&b) == 0); // TODO: comparison with zero
    return 0;
}

static char* test_odd_even_black_win() {
    printf("test_odd_even_black_win\n");
    Board b;
    // Taken from: A Knowledge-based Approach of Connect-Four
    // Black to move, white will win
    parser_read(&b,
            "b - b w - b b"
            "w - w b - w w"
            "w - b w - w b"
            "b w w b - w b"
            "w b b b - b w"
            "w w b w - b w");
    fboard2eps(&b, "build/test_odd_even_black_win.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    search(&b, &rec);
    mu_assert("error, expected black to move (tester's note)", b.turn == BLACK);
    mu_assert("error, expected black to win", rec.rating >= BETA_MAX);
    mu_assert("error, this position should be classified as an advantage for black", eval_odd_even(&b) < 0);
    return 0;
}

static char* test_odd_even_black_win2() {
    printf("test_odd_even_black_win2\n");
    Board b;
    parser_read(&b,
            "- - b w - b w"
            "w - b w - w b"
            "w - b w - w b"
            "b b w b w b b"
            "b w w b b w w"
            "b w w b w w b");
    fboard2eps(&b, "build/test_odd_even_black_win2.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    search(&b, &rec);
    stats_print(&b, &rec);
    mu_assert("error, expected black to move (tester's note)", b.turn == BLACK);
    mu_assert("error, expected black to win", rec.rating >= BETA_MAX);
    mu_assert("error, this position should be classified as an advantage for black", eval_odd_even(&b) < 0);
    return 0;
}

static char* all_tests() {
    mu_run_test(test_eval_white_win);
    mu_run_test(test_eval_black_win);

    /* TODO: implement eval_odd_even
    mu_run_test(test_odd_even_white_win);
    mu_run_test(test_odd_even_white_win2);
    mu_run_test(test_odd_even_white_win3);
    mu_run_test(test_odd_even_draw);
    mu_run_test(test_odd_even_black_win);
    mu_run_test(test_odd_even_black_win2); */

    return 0;
}

int eval_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS RUN\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
