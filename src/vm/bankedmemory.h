#ifndef BANKEDMEMORY_H
#define BANKEDMEMORY_H

#include "memory.h"

typedef struct {
    sienna_mem_t* banks;
} sienna_bankedmemory_t;

void sienna_bankedmemory_init(sienna_bankedmemory_t* banked_memory, int bank_count);
void sienna_bankedmemory_cleanup(sienna_bankedmemory_t *banked_memory);

#endif