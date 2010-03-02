#include <math.h>
#include "board.h"
#include "eval.h"

/* Defines the ply since when the evaluation function will rate a position just
 * on the criteria win, loss, and ongoing. */
#define FAST_EVAL_THRESHOLD 10

static int count_threats(Board *b) {
    int8_t threats = 0;
    for (int8_t col = 0; col < NUM_COLS; col++) {
        int8_t top = b->tops[col];
        for (int8_t row = top; row < NUM_ROWS; row++) {
            if (board_is_threat(b, WHITE, row, col)) {
                threats++;
            }
            if (board_is_threat(b, BLACK, row, col)) {
                threats--;
            }
        }
    }
    return threats;
}

float eval(Board *b, bool defer_defeat) {
    if (b->winner == WHITE) {
        // When doing a normal search, win positions all have the same value.
        // When we are trying to defer a defeat win positions that are very
        // close are more valuable. This makes the AI avoid to lose very early.
        return BETA_MAX + (defer_defeat ? 42 - b->move_cnt : 0);
    } else if (b->winner == BLACK) {
        return ALPHA_MIN - (defer_defeat ? 42 - b->move_cnt : 0);
    } else if (b->move_cnt <= FAST_EVAL_THRESHOLD) {
        return eval_odd_even(b);
    } else {
        return 0;
    }
}

struct Threat {
    int8_t row;
    int8_t col;
    Player owner;
};

typedef struct Threat Threat;

// TODO: eval_odd_even is in a very rough experimental state
float eval_odd_even(Board *b) {
    Threat white_threat;
    Threat black_threat;
    white_threat.col = white_threat.row = 0;
    black_threat.col = black_threat.row = 0;
    white_threat.owner = NOBODY;
    black_threat.owner = NOBODY;
    // Collect threats
    for (int8_t col = 0; col < NUM_COLS; col++) {
        for (int8_t row = b->tops[col] + 1; row < NUM_ROWS; row++) {
            if (board_is_threat(b, WHITE, row, col)) {
                if (white_threat.owner == NOBODY) {
                    white_threat.row = row;
                    white_threat.col = col;
                    white_threat.owner = WHITE;
                } else {
                    return 0;
                }
            }
            if (board_is_threat(b, BLACK, row, col)) {
                if (black_threat.owner == NOBODY) {
                    black_threat.row = row;
                    black_threat.col = col;
                    black_threat.owner = BLACK;
                } else {
                    return 0;
                }
            }
        }
    }
    if (white_threat.owner == NOBODY && black_threat.owner == NOBODY) {
        return 0;
    }
    
    // Apply rules
    bool white_even = (white_threat.row % 2 == 0);
    bool black_odd = (black_threat.row % 2 == 1);
    // rule 1.2
    if (white_threat.owner == NOBODY && white_even) {
        return 1;
    }
    // rule 1.1
    if (black_threat.owner == NOBODY && black_odd) {
        return -1;
    }
    if (white_threat.col == black_threat.col) {
        if (white_even && black_odd) {
            // rule 2.9
            if (white_threat.row < black_threat.row) {
                return 1;
            } else {
                return -1;
            }
        } else if (white_even && !black_odd) {
            // rule 2.12
            return 1;
        } else if (!white_even && black_odd) {
            // rule 2.11
            return -1;
        } else if (!white_even && !black_odd) {
            // rule 2.10
            return 0;
        }
    } else {
        if (white_even && black_odd) {
            // rule 2.14
            return 1;
        } else if (white_even && !black_odd) {
            // rule 2.16
            return 0;
        } else if (!white_even && black_odd) {
            // rule 2.15
            return -1;
        } else if (!white_even && !black_odd) {
            // rule 2.13
            return 0;
        }
    }

    // draw or undecided
    return 0;
}
