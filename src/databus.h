#ifndef DATABUS_H
#define DATABUS_H

#include "memory.h"

typedef struct {
    sienna_mem_t memory;
} sienna_databus_t;

void sienna_databus_init(sienna_databus_t* databus);
void sienna_databus_send(sienna_databus_t* databus, int data, int address);
int sienna_databus_fetch(sienna_databus_t* databus, int address);
void sienna_databus_cleanup(sienna_databus_t* databus);

#endif