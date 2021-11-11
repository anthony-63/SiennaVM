#include "assembler.h"

int main() {
    sienna_assembler_t assembler;
    //sienna_assembler_assemblestr(&assembler, "movir $0A, %ar\nmovrr %ar, %br");
    sienna_assembler_assemble(&assembler, "programs/main.svm", "programs/main.sbin");
    return 0;
}