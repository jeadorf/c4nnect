#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "board.h"
#include "parser.h"
#include "util.h"
#include "board2eps.h"

static char* test_board_put() {
    printf("test_board_put\n");
    Board b;
    board_init(&b);
    fboard2eps(&b, "test_board_put-1.eps");
    board_put(&b, 3);
    fboard2eps(&b, "test_board_put-2.eps");
    mu_assert("error, could not place white piece", board_get(&b, 0, 3) == WHITE);
    board_put(&b, 3);
    fboard2eps(&b, "test_board_put-3.eps");
    mu_assert("error, could not place black piece", board_get(&b, 1, 3) == BLACK);
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
    fboard2eps(&b, "test_board_undo-1.eps");
    board_put(&b, 3);
    fboard2eps(&b, "test_board_undo-2.eps");
    mu_assert("error, game should be won by BLACK", b.winner == BLACK);
    board_undo(&b, 3);
    fboard2eps(&b, "test_board_undo-3.eps");
    mu_assert("error, game should be ongoing", b.winner == NOBODY);
    return 0;
}

static char* test_board_undo_full() {
    printf("test_board_undo_full\n");
    Board b;
    parser_read(&b,
            "- w - - - - -"
            "- b - - - - -"
            "- b - - - - -"
            "w w - - - - -"
            "w w - - - - -"
            "w b b - b - -");
    fboard2eps(&b, "test_board_undo_full-1.eps");
    mu_assert("error, column should be full", board_column_full(&b, 1));
    board_undo(&b, 1);
    fboard2eps(&b, "test_board_undo_full-2.eps");
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
            "w - w b w b -");
    fboard2eps(&b, "test_board_undo_win-1.eps");
    board_put(&b, 5);
    fboard2eps(&b, "test_board_undo_win-2.eps");
    mu_assert("error, game should be won by WHITE", b.winner == WHITE);
    board_undo(&b, 5);
    fboard2eps(&b, "test_board_undo_win-3.eps");
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
    fboard2eps(&b, "test_board_undo_multiple-1.eps");
    board_put(&b, 5); // white
    fboard2eps(&b, "test_board_undo_multiple-2.eps");
    board_put(&b, 1); // black
    fboard2eps(&b, "test_board_undo_multiple-3.eps");
    board_undo(&b, 1); // black
    fboard2eps(&b, "test_board_undo_multiple-4.eps");
    board_undo(&b, 5); // white
    fboard2eps(&b, "test_board_undo_multiple-5.eps");
    mu_assert("error, expected no piece here", board_get_top(&b, 1) == NOBODY);
    mu_assert("error, expected no piece here", board_get(&b, 1, 5) == NOBODY);
    return 0;
}

static char* test_board_column_full() {
    printf("test_board_column_full\n");
    int r;
    Board b;
    board_init(&b);
    fboard2eps(&b, "test_board_column_full-1.eps");
    for (r = 0; r < NUM_ROWS; r++) {
        mu_assert("error, column cannot be full", !board_column_full(&b, 3));
        board_put(&b, 3);
    }
    fboard2eps(&b, "test_board_column_full-2.eps");
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
    fboard2eps(&b, "test_board_full-2.eps");
    mu_assert("error, board should be full", board_full(&b));
    mu_assert("error, game should be drawn", b.winner == NOBODY);
    mu_assert("error, move_cnt should match number of pieces", b.move_cnt == NUM_ROWS * NUM_COLS);
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
    fboard2eps(&b, "test_board_not_full.eps");
    mu_assert("error, board should not be full", !board_full(&b));
    mu_assert("error, game should be drawn", b.winner == NOBODY);
    return 0;
}

static char* test_board_move_wins_col() {
    printf("test_board_move_wins_col\n");
    Board b;
    board_init(&b);
    fboard2eps(&b, "test_board_move_wins_col-1.eps");
    for (int8_t i = 0; i <= 6; i++) {
        mu_assert("error, game is still drawn", b.winner == NOBODY);
        if (i % 2 == 0) {
            board_put(&b, 3); // white
        } else {
            board_put(&b, 2); // black
        }
    }
    fboard2eps(&b, "test_board_move_wins_col-2.eps");
    mu_assert("error, game is won", b.winner == WHITE);
    return 0;
}

static char* test_board_move_wins_row() {
    printf("test_board_move_wins_row\n");
    Board b;
    board_init(&b);
    fboard2eps(&b, "test_board_move_wins_row-1.eps");
    for (int8_t i = 0; i <= 6; i++) {
        mu_assert("error, game is still drawn", b.winner == NOBODY);
        if (i % 2 == 0) {
            board_put(&b, i / 2); // white
        } else {
            board_put(&b, 6); // black
        }
    }
    fboard2eps(&b, "test_board_move_wins_row-2.eps");
    mu_assert("error, game is won", b.winner == WHITE);
    return 0;
}

static char* test_board_move_wins_row2() {
    printf("test_board_move_wins_row2\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- - - - - - -"
            "w b - - - - -"
            "w w - - - - -"
            "w w b - b b -");
    fboard2eps(&b, "test_board_move_wins_row2-1.eps");
    mu_assert("error, game is still drawn", b.winner == NOBODY);
    board_put(&b, 3);
    fboard2eps(&b, "test_board_move_wins_row2-2.eps");
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
    fboard2eps(&b, "test_board_move_wins_diagup.eps");
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
    fboard2eps(&b, "test_board_move_wins_diagup2.eps");
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
    fboard2eps(&b, "test_board_move_wins_diagdown.eps");
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
    fboard2eps(&b, "test_board_move_wins_diagdown2.eps");
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
    mu_assert("error, expected boards to match", 0 == memcmp(&b1, &b2, sizeof (Board)));
    mu_assert("error, expected boards to match", 0 == memcmp(&b1, &b3, sizeof (Board)));
    Board b4;
    board_init(&b4);
    board_put(&b4, 3);
    mu_assert("error, expected boards to differ", 0 != memcmp(&b1, &b4, sizeof (Board)));
    return 0;
}

static char* test_board_is_threat() {
    printf("test_board_is_threat\n");
    Board b;
    parser_read(&b,
            "- - - - - - -"
            "- - - - - - -"
            "- b - - - - -"
            "w w - - - - -"
            "w w - - - - -"
            "w b b - b - -");
    fboard2eps(&b, "test_board_is_threat.eps");
    Board b2;
    memcpy(&b2, &b, sizeof (Board));
    mu_assert("error, should be a white threat", board_is_threat(&b, WHITE, 3, 0));
    mu_assert("error, should be a white threat, not a black one", !board_is_threat(&b, BLACK, 3, 0));
    mu_assert("error, should be a black threat", board_is_threat(&b, BLACK, 0, 3));
    mu_assert("error, should be a black threat, not a white one", !board_is_threat(&b, WHITE, 0, 3));
    mu_assert("error, test should not alter board", 0 == memcmp(&b, &b2, sizeof (Board)));
    return 0;
}

static char* test_board_decode_empty() {
    printf("test_board_decode_empty\n");
    Board b1;
    board_init(&b1);
    Board b2;
    board_decode(&b2, 0x0);
    mu_assert("error, expected boards to match", 0 == memcmp(&b1, &b2, sizeof (Board)));
    return 0;
}

static char* test_board_decode() {
    printf("test_board_decode\n");
    Board b1;
    parser_read(&b1,
            "- - - - - - -"
            "- - - - - - -"
            "b b - - - - -"
            "w w - - - - -"
            "w w - - - - -"
            "w b b - b - w");
    fboard2eps(&b1, "test_board_decode-1.eps");
    Board b2;
    uint64_t n = 0x9000041001061308;
    board_decode(&b2, n);
    fboard2eps(&b2, "test_board_decode-2.eps");
    mu_assert("error, expected codes to match", b1.code == b2.code);
    mu_assert("error, expected boards to match", 0 == memcmp(&b1, &b2, sizeof (Board)));
    return 0;
}

static char* test_board_code() {
    printf("test_board_code\n");
    Board b;
    board_init(&b);
    parser_read(&b,
            "- - - - - - - "
            "- - - - - - - "
            "- - - - - - - "
            "- - - - - - - "
            "- - - w - - - "
            "- - - w - - b ");
    fboard2eps(&b, "test_board_code.eps");
    char s[16];
    sprintf(s, "%lX", b.code);
    mu_assert("expected code to be of length 16", strlen(s) == 16);
    return 0;
}

static char* test_board_encode_incremental() {
    printf("test_board_encode_incremental\n");
    Board b, b2;
    board_init(&b);
    mu_assert("error, expected board to map to code 0x0", 0UL == b.code);
    board_put(&b, 3);
    board_decode(&b2, b.code);
    mu_assert("error, expected board code to match encode output", b.code == b2.code);
    board_put(&b, 5);
    board_decode(&b2, b.code);
    mu_assert("error, expected board code to match encode output", b.code == b2.code);
    board_undo(&b, 5);
    board_decode(&b2, b.code);
    mu_assert("error, expected board code to match encode output", b.code == b2.code);
    board_undo(&b, 3);
    board_decode(&b2, b.code);
    mu_assert("error, expected board code to match encode output", b.code == b2.code);
    return 0;
}

static char* test_board_move_cnt() {
    printf("test_board_move_cnt\n");
    Board b;
    board_init(&b);
    mu_assert("move_cnt of empty board should be zero", b.move_cnt == 0);
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
    mu_run_test(test_board_is_threat);
    mu_run_test(test_board_compare);
    mu_run_test(test_board_decode_empty);
    mu_run_test(test_board_decode);
    mu_run_test(test_board_encode_incremental);
    mu_run_test(test_board_code);
    mu_run_test(test_board_move_cnt);
    return 0;
}

int board_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS RUN\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
