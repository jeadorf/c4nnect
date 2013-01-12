#ifndef _UCITABLE_H
#define _UCITABLE_H

#include <stdio.h>
#include <stdint.h>

#define UCITABLE_SIZE 104729

/*! A slot in the UCI ML opening database table. A slot is marked empty if and
 * only if the 64-bit unsigned integer that encodes the board position is zero.
 * This hash table uses hashing with chaining. */
struct UCITEntry {
    /*! Encoded board position. */
    uint64_t code;
    /*! Relative rating with respect to the active player. */
    float rating;
    /*! Stores the next entry in the linked list. */
    struct UCITEntry *next;
};

typedef struct UCITEntry UCITEntry;

UCITEntry* ucitable_lookup(uint64_t code);

/*! Unmarshals a transposition table from a file like provided online at
 * http://archive.ics.uci.edu/ml/machine-learning-databases/connect-4 */
void ucitable_read(FILE *f);

#endif	/* _UCITABLE_H */
