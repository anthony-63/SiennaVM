#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "registers.h"
#include "bankeddatabus.h"
#include "stack.h"

typedef struct {
    sienna_stack_t* stack;
    sienna_registers_t registers;
    sienna_bankeddatabus_t databus;
} sienna_processor_t;

void sienna_processor_init(sienna_processor_t* processor, int bank_count);
void sienna_processor_run(sienna_processor_t* processor, int location, int debug);
void sienna_processor_load(sienna_processor_t* processor, int* program, int psize, int bank, int location);
void sienna_processor_loadbin(sienna_processor_t* processor, char* bin_file, int bank, int location);
#endif