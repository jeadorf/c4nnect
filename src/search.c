#include <malloc.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "eval.h"
#include "search.h"
#include "stats.h"
#include "ttable.h"
#include "ucitable.h"
#include "util.h"

#ifdef DEBUG
void stats_print(Board *b, Variation *var, SearchRecord *rec);
#endif

extern TTEntry ttable[];
extern UCITEntry ucitable[];

#define DEFAULT_MAX_DEPTH 10

void searchcfg_init(SearchCfg *cfg) {
    cfg->abort_on_timeout = false;
    cfg->defer_defeat = false;
    cfg->ttcuts_enabled = true;
    cfg->max_depth = DEFAULT_MAX_DEPTH;
}

// Simple move ordering, first walk along the principal variation then start
// with checking the moves in the center and then circle to the outer columns

void generate_moves(Variation *var, int depth, int8_t *moves) {
    int8_t i = 0, k = 0, col = 0;
    if (depth < var->length) {
        moves[0] = var->moves[depth];
        i++;
    }
    while (k < NUM_COLS) {
        col = (NUM_COLS / 2) + (k % 2 == 0 ? 1 : -1) * (k + 1) / 2;
        if (i == 0 || col != moves[0]) {
            moves[i] = col;
            i++;
        }
        k++;
    }
}

// TODO: we should report draw games, too
bool alphabeta_negamax(
        Board *b, float alpha, float beta, int8_t depth,
        SearchCfg *cfg, Variation *var, SearchRecord *rec) {
#ifndef DISABLE_TTABLE
    // TODO: Extract MACRO or method for computing the hash for a board
    uint32_t hash = b->code % TTABLE_SIZE;
    TTEntry *ttentry = &(ttable[hash]);
#endif

#ifndef DISABLE_UCITABLE
    UCITEntry *ucitentry = ucitable_lookup(b->code);
    // May NOT return variation with length 0 if this is the root of the recursion tree
    if (depth > 0 && b->move_cnt == 8) {
        if (ucitentry == NULL) {
            // no op
        } else if (ucitentry->code == b->code) {
            var->rating = ucitentry->rating;
            var->length = 0; // can't do that!
            return true;
        } else {
            puts("ERROR!\n");
        }
    }
#endif
    
    if (cfg->abort_on_timeout && clock_in_millis(clock() - rec->cpu_time) > TIME_LIMIT_PER_PLY - TIME_LIMIT_SAFETY_MARGIN) {
        return false;
    } else if (b->winner != NOBODY
            || board_full(b)
            || depth == cfg->max_depth) {
        if (depth == 0) {
            handle_error("cannot search for a move in a finished game!");
        }
        var->rating = (b->turn == WHITE ? 1 : -1) * eval(b, cfg->defer_defeat);
        var->length = 0;
        rec->eval_cnt++;
#ifndef DISABLE_TTABLE
    } else if (cfg->ttcuts_enabled && ttentry->code == b->code && depth > 0) {
        // The transposition entry does not contain information about the move
        // that lead to the position, so we cannot check the transposition table
        // at the root of the search tree.
        var->rating = ttentry->rating;
        var->length = 0;
        rec->ttcut_cnt++;
#endif
    } else {
#ifndef DISABLE_TTABLE
        if (cfg->ttcuts_enabled && ttentry->code != b->code && ttentry->code != 0) {
            rec->ttrcoll_cnt++;
        }
#endif

        float bestval = alpha;
        int bestcol = -1;
        Variation pv = *var;

        // FIXME: There might be invalid boards that are not classified
        //        correctly because of transposition tables!??
        int8_t moves[NUM_COLS];
        generate_moves(var, depth, moves);
        for (int8_t i = 0; i < NUM_COLS; i++) {
            int8_t col = moves[i];
            if (!board_column_full(b, col)) {

                // Search subposition
                // TODO: check whether undo mechanism does any good
                board_put(b, col);
                bool completed = alphabeta_negamax(b, -beta, -bestval, depth + 1, cfg, var, rec);
                board_undo(b, col);

                // Check on timeout, must happen after undo operation
                if (!completed) {
                    return false;
                }

                var->rating *= -1;

                if (var->rating > bestval || bestcol == -1) {
                    // Found a move that is either better than all other moves
                    bestval = var->rating;
                    bestcol = col;
                    // Construct principal variation. Append the PV of the subtree
                    // to the PV that led to this node.
                    // TODO: need to profile this, Variation data structure probably too big
                    pv.moves[0] = col;
                    memcpy(pv.moves + 1, var->moves, var->length * sizeof (col));
                    pv.length = var->length + 1;
                }

#ifndef DISABLE_ABCUTS
                // Check for a beta cutoff
                if (bestval >= beta) {
                    rec->abcut_cnt++;
                    break;
                }
#endif
            }
        }

        *var = pv;
        var->rating = bestval;
#ifndef DISABLE_TTABLE
        if (winner_identified(var->rating)) {
            ttentry->code = b->code;
            ttentry->rating = var->rating;
            // TODO: this might count some of the entries twice?
            rec->ttadd_cnt++;
        }
#endif
    }

    rec->visited_cnt++;
    rec->max_depth = cfg->max_depth;
    if (depth > rec->reached_depth) {
        rec->reached_depth = depth;
    }

    return true;
}

int8_t searchm(Board * b) {
    Variation var;
    variation_init(&var);
    SearchRecord rec;
    searchrecord_init(&rec);
    search(b, &var, &rec);
    return var.moves[0];
}

void search(Board *b, Variation *var, SearchRecord *rec) {
    clock_t start_time = clock();
    rec->cpu_time = start_time;
    // The iterative approach implies that reached_depth will always equal
    // max_depth.
    Variation tmp_var;
    SearchCfg cfg;
    searchcfg_init(&cfg);
    cfg.max_depth = 0;
    do {
        cfg.max_depth++;
        cfg.abort_on_timeout = cfg.max_depth > 1;
        variation_init(&tmp_var);
        if (alphabeta_negamax(b, ALPHA_MIN, BETA_MAX, 0, &cfg, &tmp_var, rec)) {
            // TODO: define exactly what reached_depth should represent
            *var = tmp_var;
            if (winner_identified(var->rating)) {
                // Defer defeats that are unavoidable. The computer should at least not to
                // lose in the next move even if the computer sees that he will lose against
                // the perfect-playing opponent. Deferring defeats does only work without
                // transposition tables for they are not cleared between subsequent
                // searches.
                if (var->rating <= ALPHA_MIN) {
                    int8_t max_depth = cfg.max_depth;
                    int8_t reached_depth = rec->reached_depth;
                    variation_init(var);
                    // TODO: improve max_depth setting when deferring defeat
                    cfg.ttcuts_enabled = false;
                    cfg.defer_defeat = true;
                    cfg.max_depth = (max_depth - 1) / 2 + 3;
                    alphabeta_negamax(b, ALPHA_MIN_DEFEAT, BETA_MAX_DEFEAT,
                            0, &cfg, var, rec);
                    // Merge search results. This is not a really satisfying solution
                    // but it saves some difficulties in handling two partial search
                    // results
                    var->rating = ALPHA_MIN;
                    rec->defeat_deferred = true;
                    rec->max_depth = max_depth;
                    rec->reached_depth = reached_depth;
                }
                break;
            }
        } else {
            // Search aborted, stick to the last complete search result
            // and there is no need for defeat deferral.
            break;
        }
    } while (cfg.max_depth < 42 - b->move_cnt);

    rec->cpu_time = clock() - rec->cpu_time;
    rec->on_time = clock_in_millis(rec->cpu_time) < TIME_LIMIT_PER_PLY;
    rec->ttcharge = ttable_charge();
}
