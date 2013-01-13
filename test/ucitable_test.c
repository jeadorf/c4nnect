#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "minunit.h"
#include "parser.h"
#include "ucitable.h"

extern UCITEntry ucitable[];

static char* test_ucitable_read_win() {
    printf("test_ucitable_read_win\n");
    
    FILE* f = fopen("data/connect-4.data", "r");
    ucitable_read(f);
    
    Board b;
    parser_read(&b,
            "- - - b - - -"
            "- - - w - - -"
            "- - - b - - -"
            "- - - w - - -"
            "- - b b - - -"
            "- - w w - - -");
    
    UCITEntry *ucitentry = ucitable_lookup(b.code);
    mu_assert("error. wrong ucitable entry", ucitentry->code == b.code);
    mu_assert("error, expected victory for white", ucitentry->rating > 0);
             
    return 0;
}

static char* test_ucitable_read_draw() {
    printf("test_ucitable_read_draw\n");
    
    FILE* f = fopen("data/connect-4.data", "r");
    ucitable_read(f);
    
    Board b;
    parser_read(&b,
            "- - - w - - -"
            "- - - b - - -"
            "- - - b - - -"
            "- - - w - - -"
            "- - - b - - -"
            "w - - w - - b");
    
    UCITEntry *ucitentry = ucitable_lookup(b.code);
    mu_assert("error. wrong ucitable entry", ucitentry->code == b.code);
    mu_assert("error, expected draw", ucitentry->rating == 0);
             
    return 0;
}

static char* test_ucitable_read_loss() {
    printf("test_ucitable_read_loss\n");
    
    FILE* f = fopen("data/connect-4.data", "r");
    ucitable_read(f);
    
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - b - - -"
            "- w - b - - -"
            "b w w w b - -");
    
    UCITEntry *ucitentry = ucitable_lookup(b.code);
    mu_assert("error. wrong ucitable entry", ucitentry->code == b.code);
    mu_assert("error, expected victory for black", ucitentry->rating < 0);
             
    return 0;
}
   
static char* test_ucitable_lookup_none() {
    printf("test_ucitable_lookup_none\n");
    
    FILE* f = fopen("data/connect-4.data", "r");
    ucitable_read(f);
    
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- w - b - - -"
            "b w w w b - -");
    
    UCITEntry *ucitentry = ucitable_lookup(b.code);
    mu_assert("error. was expecting NULL entry", ucitentry == NULL);
             
    return 0;
}

int main() {
    mu_run_test(test_ucitable_read_win);
    mu_run_test(test_ucitable_read_draw);
    mu_run_test(test_ucitable_read_loss);
    mu_run_test(test_ucitable_lookup_none);
    return tests_failed;
}
