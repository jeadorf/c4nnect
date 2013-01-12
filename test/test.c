#include <stdlib.h>

#include "minunit.h"

#include "board_test.h"
#include "eval_test.h"
#include "parser_test.h"
#include "search_test.h"
#include "stats_test.h"
#include "ucitable_test.h"

int main() {
    board_test();
    eval_test();
    parser_test();
    search_test();
    stats_test();
    ucitable_test();
    if (tests_failed > 0) {
        puts("TESTS FAILED!");
        return (EXIT_FAILURE);
    }
    return 0;
}
