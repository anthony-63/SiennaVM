#ifndef ASSEMBLER_H
#define ASSEMBLER_H

typedef struct {
    int index;
    char* program;
    int* out;
    int oidx;
    int psize;
    int ooffs;
} sienna_assembler_t;

void sienna_assembler_assemble(sienna_assembler_t* assembler, char* input_file, char* output_file);
void sienna_assembler_assemblestr(sienna_assembler_t* assembler, char* prog);

#endif