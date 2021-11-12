#include <stdio.h>
#include <stdlib.h>
#include "processor.h"
#include "instructions.h"
int main(int argc, char** argv) {
    sienna_processor_t processor;
    sienna_processor_init(&processor, 1);
    if(argc < 2 || argc > 3) {
        printf("Usage: ./sienna(.exe) <sbin file>\n");
        exit(0);
    }
    // int prog[] = {
    //     movir, 0x04, ar, 
    //     decr, ar,
    //     cmpir, 0x00, ar,
    //     jne, 0x03,
    //     hlt
    // };
    //size bank location
    // sienna_processor_load(&processor, prog, sizeof(prog)/sizeof(prog[0]), 0, 0x0000);
    sienna_processor_loadbin(&processor, argv[1], 0, 0x0000);
    sienna_processor_run(&processor, 0x0000, 1);
    sienna_processor_cleanup(&processor);
    return 0;
}