#include <stdlib.h>
#include "memory.h"

void sienna_mem_init(sienna_mem_t* mem) {
    mem->mem = (int*)malloc(sizeof(int) * 0xFFFF);
}

void sienna_mem_cleanup(sienna_mem_t* mem) {
    free(mem->mem);
}