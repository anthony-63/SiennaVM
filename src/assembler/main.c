#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
    sienna_assembler_t assembler;
    if(argc < 3 || argc > 4) {
        printf("Usage: siennac(.exe) <in_file> <out_file>\n");
        exit(0);
    }
    //sienna_assembler_assemblestr(&assembler, "movir $0A, %ar\nmovrr %ar, %br");
    sienna_assembler_assemble(&assembler, argv[1], argv[2]);
    sienna_assembler_cleanup(&assembler);
    return 0;
}