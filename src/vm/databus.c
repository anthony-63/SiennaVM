#include "databus.h"

void sienna_databus_init(sienna_databus_t* databus){
    sienna_mem_init(&databus->memory);
}

void sienna_databus_send(sienna_databus_t* databus, int data, int address){
    databus->memory.mem[address] = data;
}

int sienna_databus_fetch(sienna_databus_t* databus, int address){
    return databus->memory.mem[address];
}

void sienna_databus_cleanup(sienna_databus_t* databus){
    sienna_mem_cleanup(&(databus->memory));
}
