#ifndef _TTABLE_H
#define	_TTABLE_H

#include <stdint.h>

#define TRANSPOS_SIZE 524287 // 104729

struct TTEntry {
    uint64_t code;
    float rating;
    int8_t move;
};

typedef struct TTEntry TTEntry;

#endif	/* _TTABLE_H */

