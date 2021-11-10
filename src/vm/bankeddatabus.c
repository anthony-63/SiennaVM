#include "bankeddatabus.h"


void sienna_bankeddatabus_init(sienna_bankeddatabus_t* databus, int bank_count){
    sienna_bankedmemory_init(&(databus->banked_memory), bank_count);
}

void sienna_bankeddatabus_send(sienna_bankeddatabus_t* databus, int bank, int data, int address){
    databus->banked_memory.banks[bank].mem[address] = data;
}

int sienna_bankeddatabus_fetch(sienna_bankeddatabus_t* databus, int bank, int address){
    return databus->banked_memory.banks[bank].mem[address];
}

void sienna_bankeddatabus_cleanup(sienna_bankeddatabus_t* databus){
    sienna_bankedmemory_cleanup(&(databus->banked_memory));
}