#include <stdlib.h>
#include <string.h>
#include "registers.h"

void sienna_registers_init(sienna_registers_t* registers){
    registers->regs = (int*)malloc(sizeof(int)*REGISTER_COUNT);
    memset(registers->regs, 0, sizeof(registers->regs));
}

void sienna_registers_resetflags(sienna_registers_t* registers){
    registers->regs[jr] = 0;
}

void sienna_registers_setregister(sienna_registers_t* registers, int reg, int val){
    registers->regs[reg] = val;
}

int sienna_registers_getregister(sienna_registers_t* registers, int reg){
    return registers->regs[reg];
}

void sienna_registers_cleanup(sienna_registers_t* registers){
    free(registers->regs);
}
