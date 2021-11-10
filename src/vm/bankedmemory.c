#include <stdlib.h>
#include "bankedmemory.h"

void sienna_bankedmemory_init(sienna_bankedmemory_t* banked_memory, int bank_count){
    banked_memory->banks = (sienna_mem_t*)malloc(sizeof(sienna_mem_t)*bank_count);
    for(int i = 0; i < bank_count; i++) {
        sienna_mem_init(&(banked_memory->banks[i]));
    }
}

void sienna_bankedmemory_cleanup(sienna_bankedmemory_t* banked_memory){
    for(int i = 0; i < sizeof(banked_memory->banks) / sizeof(sienna_mem_t); i++) {
        sienna_mem_cleanup(&(banked_memory->banks[i]));
    }
}
