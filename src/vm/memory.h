#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
    int *mem;
} sienna_mem_t;

void sienna_mem_init(sienna_mem_t* memory);
void sienna_mem_cleanup(sienna_mem_t* memory);

#endif