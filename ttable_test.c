#include <stdlib.h>
#include <stdio.h>

#include "minunit.h"
#include "board.h"
#include "parser.h"
#include "ttable.h"

extern TTEntry ttable[];

static char* test_ttable_read_from_uciml_win() {
    printf("test_ttable_read_from_uciml_win\n");
    
    FILE* f = fopen("connect-4.data", "r");
    ttable_read_uciml(f);
    
    Board b;
    parser_read(&b,
            "- - - b - - -"
            "- - - w - - -"
            "- - - b - - -"
            "- - - w - - -"
            "- - b b - - -"
            "- - w w - - -");
    int32_t hash = b.code % TTABLE_SIZE;
    TTEntry *ttentry = &(ttable[hash]);
    mu_assert("error. wrong ttable entry", ttentry->code == b.code);
    mu_assert("error, expected victory for white", ttentry->rating > 0); // TODO:
             
    return 0;
}

static char* test_ttable_read_from_uciml_draw() {
    printf("test_ttable_read_from_uciml_draw\n");
    
    FILE* f = fopen("connect-4.data", "r");
    ttable_read_uciml(f);
    
    Board b;
    parser_read(&b,
            "- - - w - - -"
            "- - - b - - -"
            "- - - b - - -"
            "- - - w - - -"
            "- - - b - - -"
            "w - - w - - b");
    int32_t hash = b.code % TTABLE_SIZE;
    TTEntry *ttentry = &(ttable[hash]);
    mu_assert("error. wrong ttable entry", ttentry->code == b.code);
    mu_assert("error, expected victory for white", ttentry->rating == 0); // TODO:
             
    return 0;
}

static char* test_ttable_read_from_uciml_loss() {
    printf("test_ttable_read_from_uciml_loss\n");
    
    FILE* f = fopen("connect-4.data", "r");
    ttable_read_uciml(f);
    
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - b - - -"
            "- w - b - - -"
            "b w w w b - -");
    int32_t hash = b.code % TTABLE_SIZE;
    TTEntry *ttentry = &(ttable[hash]);
    mu_assert("error. wrong ttable entry", ttentry->code == b.code);
    mu_assert("error, expected victory for white", ttentry->rating < 0); // TODO:
             
    return 0;
}
   
static char* all_tests() {
    mu_run_test(test_ttable_read_from_uciml_win);
    mu_run_test(test_ttable_read_from_uciml_draw);
    mu_run_test(test_ttable_read_from_uciml_loss);
    return 0;
}

int ttable_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS RUN\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
