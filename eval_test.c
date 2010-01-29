#include <stdio.h>
#include <math.h>
#include "board.h"
#include "eval_test.h"
#include "minunit.h"
#include "parser.h"
#include "eval.h"
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
    fboard2eps(&b, "build/test_eval_black_win.eps");
    mu_assert("error, evaluation value should be negative", eval(&b) < 0);
    mu_assert("error, evaluation value should be negative "
              "with big absolute value", eval(&b) <= ALPHA_MIN);
    return 0;
}

static char* all_tests() {
    mu_run_test(test_eval_white_win);
    mu_run_test(test_eval_black_win);
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
