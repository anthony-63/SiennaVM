#ifndef BANKEDDATABUS_H
#define BANKEDDATABUS_H

#include "bankedmemory.h"

typedef struct {
    sienna_bankedmemory_t banked_memory;
} sienna_bankeddatabus_t;

void sienna_bankeddatabus_init(sienna_bankeddatabus_t* databus, int bank_count);
void sienna_bankeddatabus_send(sienna_bankeddatabus_t* databus, int bank, int data, int address);
int sienna_bankeddatabus_fetch(sienna_bankeddatabus_t* databus, int bank, int address);
void sienna_bankeddatabus_cleanup(sienna_bankeddatabus_t* databus);

#endif