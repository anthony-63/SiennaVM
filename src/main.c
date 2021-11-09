#include <stdio.h>
#include "processor.h"
#include "instructions.h";
int main() {
    sienna_processor_t processor;
    sienna_processor_init(&processor, 1);
    int prog[] = {
        movir, 0x10, ar,
        hlt,
    };
    //size bank location
    sienna_processor_load(&processor, prog, sizeof(prog)/sizeof(prog[0]), 0, 0x0000);
    sienna_processor_run(&processor, 0x0000, 1);
    return 0;
}