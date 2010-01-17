#include <stdio.h>
#include <math.h>
#include "board.h"
#include "eval_test.h"
#include "minunit.h"
#include "parser.h"
#include "eval.h"

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
    mu_assert("error, evaluation value should be positive", eval(&b) >= 0);
    mu_assert("error, evaluation value should be big", eval(&b) >= BETA_MAX);
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
    mu_assert("error, evaluation value should be negative", eval(&b) < 0);
    mu_assert("error, evaluation value should be negative "
              "with big absolute value", eval(&b) <= ALPHA_MIN);
    return 0;
}

static char* test_eval_center_bonus() {
    printf("test_eval_center_bonus\n");
    Board b1;
    parser_read(&b1,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "b - - w - - -");
    Board b2;
    parser_read(&b2,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "b w - - - - -");
    mu_assert("error, center moves should be rewarded", eval(&b1) > eval(&b2));
    return 0;
}

static char* all_tests() {
    mu_run_test(test_eval_white_win);
    mu_run_test(test_eval_black_win);
    mu_run_test(test_eval_center_bonus);
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
