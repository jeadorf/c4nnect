#include <math.h>
#include <stdio.h>

#include "board.h"
#include "board2eps.h"
#include "eval.h"
#include "minunit.h"
#include "parser.h"
#include "search.h"
#include "util.h"

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
    fboard2eps(&b, "test_collect_principal_variation.eps");
    Variation var;
    variation_init(&var);
    SearchRecord rec;
    searchrecord_init(&rec);
    // Must disable transposition tables here, since they might lead to the
    // case that the principal variation is not computed for more than one
    // ply.
    SearchCfg cfg;
    searchcfg_init(&cfg);
    cfg.ttcuts_enabled = false;
    cfg.max_depth = 3;
    alphabeta_negamax(&b, ALPHA_MIN, BETA_MAX, 0, &cfg, &var, &rec);
    mu_assert("error, expected principal variation to be longer", var.length >= 2);
    mu_assert("error, expected saving move", var.moves[0] == 3);
    mu_assert("error, expected saving move", var.moves[1] == 3);
    return 0;
}

static char* test_search_depth() {
    printf("test_search_depth\n");
    Board b;
    board_init(&b);
    fboard2eps(&b, "test_search_depth.eps");
    Variation var;
    variation_init(&var);
    SearchRecord rec;
    searchrecord_init(&rec);
    // Must disable transposition tables here
    SearchCfg cfg;
    searchcfg_init(&cfg);
    cfg.ttcuts_enabled = false;
    cfg.max_depth = 2;
    alphabeta_negamax(&b, ALPHA_MIN, BETA_MAX, 0, &cfg, &var, &rec);
    mu_assert("error, unexpected value for max_depth", rec.max_depth == 2);
    mu_assert("error, unexpected value for reached_depth", rec.reached_depth == 2);
    return 0;
}

int main() {
    mu_run_test(test_collect_principal_variation);
    mu_run_test(test_search_depth);
    return tests_failed;
}
