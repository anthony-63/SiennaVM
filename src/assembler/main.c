#include "assembler.h"

int main() {
    sienna_assembler_t assembler;
    sienna_assembler_assemblestr(&assembler, "movir $0A, %ar");
    return 0;
}