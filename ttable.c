#include "ttable.h"

TTEntry ttable[TTABLE_SIZE];

float ttable_charge() {
    return ((float) ttable_entry_cnt()) / TTABLE_SIZE;
}

int32_t ttable_entry_cnt() {
    int32_t fill_cnt = 0;
    for (int i = 0; i < TTABLE_SIZE; i++) {
        if (ttable[i].code != 0) {
            fill_cnt++;
        }
    }
    return fill_cnt;
}
