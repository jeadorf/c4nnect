#ifndef _TTABLE_H
#define	_TTABLE_H

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

#endif	/* _TTABLE_H */
