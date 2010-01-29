#include <stdio.h>
#include <math.h>
#include "board.h"
#include "minunit.h"
#include "parser.h"
#include "stats_test.h"
#include "board2eps.h"
#include "search.h"
#include "eval.h"

static char* test_collect_principal_variation() {
    printf("test_collect_principal_variation\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - b - -"
            "- - - - b - -"
            "w w w - w - -"
            "w b b - b - -");
    fboard2eps(&b, "build/test_collect_principal_variation.eps");
    SearchRecord rec;
    searchrecord_init(&rec);
    // Must disable transposition tables here, since they might lead to the
    // case that the principal variation is not computed for more than one
    // ply.
    alphabeta_negamax(&b, ALPHA_MIN, BETA_MAX, 0, 2, false, &rec);
    mu_assert("error, expected principal variation to be longer", rec.pv.length >= 2);
    mu_assert("error, expected first move of principal variation and search record to match ",
            rec.pv.moves[0] == rec.move);
    mu_assert("error, expected saving move", rec.pv.moves[0] == 3);
    mu_assert("error, expected saving move", rec.pv.moves[1] == 3);
    return 0;
}

static char* all_tests() {
    mu_run_test(test_collect_principal_variation);
    return 0;
}

int stats_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS RUN\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}

