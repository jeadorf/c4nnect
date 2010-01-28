#ifndef _TTABLE_H
#define	_TTABLE_H

#include <stdint.h>

#define TTABLE_SIZE 104729

struct TTEntry {
    uint64_t code;
    float rating;
};

typedef struct TTEntry TTEntry;

#endif	/* _TTABLE_H */

