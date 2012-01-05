#ifndef _TTABLE_H
#define _TTABLE_H

#include <stdint.h>

#define TTABLE_SIZE 104729

struct TTEntry {
    uint64_t code;
    float rating;
};

typedef struct TTEntry TTEntry;

/*! Calculates the filling percentage of the transposition table. Example:
 * 0.75 means that 75% of the table is filled up. */
float ttable_charge();

/*! Counts the number of entries in the transposition table */
int32_t ttable_entry_cnt();

// TODO: improve serialization mechanism

/*! Unmarshals a transposition table from a file. This function is most likely
 * not portable. Changes to TTABLE_SIZE will cause malfunction. */
void ttable_read(FILE *f);

/*! Marshals a transposition table to a file. This function is most likely
 * not portable. Changes to TTABLE_SIZE will cause malfunction. */
void ttable_write(FILE *f);

/*! Unmarshals a transposition table from a file formatted as in 
 * http://archive.ics.uci.edu/ml/machine-learning-databases/connect-4/connect-4.names */
void ttable_read_uciml(FILE *f);

#endif	/* _TTABLE_H */
