#ifndef REGISTERS_H
#define REGISTERS_H

#define ip 0
#define mb 2
#define ar 3
#define br 4
#define cr 5
#define dr 6
#define er 7
#define fr 8
#define gr 9
#define hr 10
#define zr 11
#define jr 12
#define tr 13

#define REGISTER_COUNT 14

typedef struct {
    int *regs;
} sienna_registers_t;

void sienna_registers_init(sienna_registers_t* registers);
void sienna_registers_resetflags(sienna_registers_t* registers);
void sienna_registers_setregister(sienna_registers_t* registers, int reg, int val);
int sienna_registers_getregister(sienna_registers_t* registers, int reg);
#endif