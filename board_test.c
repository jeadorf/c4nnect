#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "board.h"
#include "parser.h"

static char* test_board_put() {
    printf("test_board_put\n");
    Board b;
    board_init(&b);
    board_put(&b, BLACK, 3);
    mu_assert("error, could not place white piece", board_get(&b, 0, 3) == BLACK);
    mu_assert("error, slot should be empty", board_get(&b, 0, 0) == NOBODY);
    return 0;
}

static char* test_board_undo() {
    printf("test_board_undo\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- b - - - - -"
            "w w - - - - -"
            "w w - - - - -"
            "w b b - b - -");
    board_put(&b, BLACK, 3);
    mu_assert("error, game should be won by BLACK", b.winner == BLACK);
    board_undo(&b, 3);
    mu_assert("error, game should be ongoing", b.winner == NOBODY);
    return 0;
}

static char* test_board_undo_full() {
    printf("test_board_undo\n");
    Board b;
    parser_read(&b,
            "- w - - - - -"
            "- b - - - - -"
            "- b - - - - -"
            "w w - - - - -"
            "w w - - - - -"
            "w b b - b - -");
    mu_assert("error, column should be full", board_column_full(&b, 1));
    board_undo(&b, 1);
    mu_assert("error, column should no longer be full", !board_column_full(&b, 1));
    return 0;
}

static char* test_board_undo_win() {
    printf("test_board_undo_win\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - w b -"
            "- - - w b b -"
            "- - w b w b -");
    board_put(&b, WHITE, 5);
    mu_assert("error, game should be won by WHITE", b.winner == WHITE);
    board_undo(&b, 5);
    mu_assert("error, game should be ongoing", b.winner == NOBODY);
    return 0;
}

static char* test_board_undo_multiple() {
    printf("test_board_undo_multiple\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - w b - -"
            "- - w b w b -");
    board_put(&b, WHITE, 5);
    board_put(&b, BLACK, 1);
    board_undo(&b, 1);
    board_undo(&b, 5);
    mu_assert("error, expected no piece here", board_get_top(&b, 1) == NOBODY);
    mu_assert("error, expected no piece here", board_get(&b, 1, 5) == NOBODY);
    return 0;
}

static char* test_board_column_full() {
    printf("test_board_column_full\n");
    int r;
    Board b;
    board_init(&b);
    for (r = 0; r < NUM_ROWS; r++) {
        mu_assert("error, column cannot be full", !board_column_full(&b, 3));
        board_put(&b, BLACK, 3);
    }
    mu_assert("error, column must be full", board_column_full(&b, 3));
    return 0;
}

static char* test_board_full() {
    printf("test_board_full\n");
    Board b;
    parser_read(&b,
            "b w b w b w b"
            "w b b b w b w"
            "b b w b w b w"
            "w w w b b b w"
            "w w b w w w b"
            "w b b w b b w");
    mu_assert("error, board should be full", board_full(&b));
    mu_assert("error, game should be drawn", b.winner == NOBODY);
    return 0;
}

static char* test_board_not_full() {
    printf("test_board_not_full\n");
    Board b;
    parser_read(&b,
            "b w - w b w b"
            "w b b b w b w"
            "b b w b w b w"
            "w w w b b b w"
            "w w b w w w b"
            "w b b w b b w");
    mu_assert("error, board should not be full", !board_full(&b));
    mu_assert("error, game should be drawn", b.winner == NOBODY);
    return 0;
}

static char* test_board_move_wins_col() {
    printf("test_board_move_wins_col\n");
    Board b;
    board_init(&b);
    for (int i = 0; i < 4; i++) {
        mu_assert("error, game is still drawn", b.winner == NOBODY);
        board_put(&b, BLACK, 3);
    }
    mu_assert("error, game is won", b.winner == BLACK);
    return 0;
}

static char* test_board_move_wins_row() {
    printf("test_board_move_wins_row\n");
    Board b;
    board_init(&b);
    for (int i = 1; i < 5; i++) {
        mu_assert("error, game is still drawn", b.winner == NOBODY);
        board_put(&b, BLACK, i);
    }
    mu_assert("error, game is won", b.winner == BLACK);
    return 0;
}

static char* test_board_move_wins_row2() {
    printf("test_board_move_wins_row2\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- b - - - - -"
            "w w - - - - -"
            "w w - - - - -"
            "w b b - b b -");
    mu_assert("error, game is still drawn", b.winner == NOBODY);
    board_put(&b, BLACK, 3);
    mu_assert("error, black should have won", b.winner == BLACK);
    return 0;
}

static char* test_board_move_wins_diagup() {
    printf("test_board_move_wins_diagup\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - w -"
            "- - - - w b -"
            "- - - w b b -"
            "- - w b w b -");
    mu_assert("error, white should have won", b.winner == WHITE);
    return 0;
}

static char* test_board_move_wins_diagup2() {
    printf("test_board_move_wins_diagup2\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - b -"
            "- - - - b w -"
            "- - - b w w -"
            "- - b w b w -");
    mu_assert("error, black should have won", b.winner == BLACK);
    return 0;
}

static char* test_board_move_wins_diagdown() {
    printf("test_board_move_wins_diagdown\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - w - - - -"
            "- - b w - - -"
            "- - b b w - -"
            "- - b w b w -");
    mu_assert("error, white should have won", b.winner == WHITE);
    return 0;
}

static char* test_board_move_wins_diagdown2() {
    printf("test_board_move_wins_diagdown2\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - b - - - -"
            "- - w b - - -"
            "- - w w b - -"
            "- - w b w b -");
    mu_assert("error, black should have won", b.winner == BLACK);
    return 0;
}

static char* test_board_compare() {
    printf("test_board_compare\n");
    Board b1;
    parser_read(&b1,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -");
    Board b2;
    board_init(&b2);
    Board b3;
    board_init(&b3);
    mu_assert("error, expected boards to match", 0 == memcmp(&b1, &b2, sizeof(Board)));
    mu_assert("error, expected boards to match", 0 == memcmp(&b1, &b3, sizeof(Board)));
    Board b4;
    board_init(&b4);
    board_put(&b4, WHITE, 3);
    mu_assert("error, expected boards to differ", 0 != memcmp(&b1, &b4, sizeof(Board)));
    return 0;
}

static char* all_tests() {
    mu_run_test(test_board_put);
    mu_run_test(test_board_undo);
    mu_run_test(test_board_undo_full);
    mu_run_test(test_board_undo_win);
    mu_run_test(test_board_undo_multiple);
    mu_run_test(test_board_full);
    mu_run_test(test_board_not_full);
    mu_run_test(test_board_column_full);
    mu_run_test(test_board_move_wins_col);
    mu_run_test(test_board_move_wins_row);
    mu_run_test(test_board_move_wins_row2);
    mu_run_test(test_board_move_wins_diagup);
    mu_run_test(test_board_move_wins_diagup2);
    mu_run_test(test_board_move_wins_diagdown);
    mu_run_test(test_board_move_wins_diagdown2);
    mu_run_test(test_board_compare);
    return 0;
}

int board_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
