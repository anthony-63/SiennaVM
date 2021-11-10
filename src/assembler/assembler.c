#include "assembler.h"
#include "../vm/instructions.h"
#include "../vm/registers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(X) assembler->program[assembler->index] == X
#define CURRENT assembler->program[assembler->index]
#define IS_HEX(X) ((int)X >= (int)'0' && (int)X <= (int)'f') && ((int)X >= (int)'A' && (int)X <= (int)'F')
#define CHECK_INST(X) assembler->program[assembler->index++] == X[0] && assembler->program[assembler->index++] == X[1] && assembler->program[assembler->index++] == X[2]
#define STEP assembler->index += 1
#define PUSH_OUT(X) assembler->out[assembler->oidx++] = X

/*
ip
mb
ar
br
cr
dr
er
fr
gr
hr
zr
jr
*/


void expected(sienna_assembler_t* assembler, char ex) {
    printf("FATAL: Expected a '%c', current char '%c', at index %d\n", ex, CURRENT, assembler->index);
    exit(-1);
}
void invalid_type(sienna_assembler_t* assembler, char* type){
    printf("FATAL: Invalid type '%s', current char '%c', at index %d\n", type, CURRENT, assembler->index);
}
void expected_type(sienna_assembler_t* assembler, char* type) {
    printf("FATAL: Expected type '%s', current char '%c', at index %d\n", type, CURRENT, assembler->index);
}
void expected_type_but_got(char* type, char* got) {
    printf("FATAL: Expected type '%s' but got '%s'\n", type, got);
}
int get_reg_index(sienna_assembler_t* assembler, char* reg) {
    if     (strcmp(reg, "ip") == 0) return ip;
    else if(strcmp(reg, "mb") == 0) return mb;
    else if(strcmp(reg, "ar") == 0) return ar;
    else if(strcmp(reg, "br") == 0) return br;
    else if(strcmp(reg, "cr") == 0) return cr;
    else if(strcmp(reg, "dr") == 0) return dr;
    else if(strcmp(reg, "er") == 0) return er;
    else if(strcmp(reg, "fr") == 0) return fr;
    else if(strcmp(reg, "gr") == 0) return gr;
    else if(strcmp(reg, "hr") == 0) return hr;
    else if(strcmp(reg, "zr") == 0) return zr;
    else if(strcmp(reg, "jr") == 0) return jr;
    else { expected_type_but_got("Register", reg); exit(-1);}
}

void consume_hex(sienna_assembler_t* assembler) {
    if(!CHECK('$')) {
        expected(assembler, '$');
    }
    char* hstr = (char*)malloc(sizeof(char)*0xFFFF);
    memset(hstr, 0, sizeof(hstr));
    STEP;
    int idx = 0;
    while(IS_HEX(CURRENT)) {
        hstr[idx++] = assembler->program[assembler->index++];
    }
    printf("Hex string: %s\n", hstr);
    PUSH_OUT((int)strtol(hstr, NULL, 16));
}

void consume_address(sienna_assembler_t* assembler) {
    if(!CHECK('&')) {
        expected(assembler, '&');
    }
    char* hstr = (char*)malloc(sizeof(char)*0xFFFF);
    memset(hstr, 0, sizeof(hstr));
    STEP;
    int idx = 0;
    while(IS_HEX(CURRENT)) {
        hstr[idx++] = assembler->program[assembler->index++];
    }
    printf("Hex string: %s\n", hstr);
    PUSH_OUT((int)strtol(hstr, NULL, 16));
}

void consume_register(sienna_assembler_t* assembler){
    if(!CHECK('%')) {
        expected(assembler, '%');
    }
    char* reg = (char*)malloc(sizeof(char)*2);
    memset(reg, 0, sizeof(reg));
    STEP;
    reg[0] = CURRENT;
    STEP;
    reg[1] = CURRENT;
    STEP;
    printf("Register: %s\n", reg);
    PUSH_OUT(get_reg_index(assembler, reg));
}

void consume_whitespace(sienna_assembler_t* assembler) {
    while(CHECK(' ') || CHECK('\n')) {
        STEP;
    }
}

void consume_comma(sienna_assembler_t* assembler) {
    if(!CHECK(',')) {
        expected(assembler, ',');
    } else {
        STEP;
        return;
    }
}
void consume_semi(sienna_assembler_t* assembler){
    if(!CHECK(';')){
        expected(assembler, ';');
    }else{
        STEP;
        return;
    }
}
/*
    int index;
    char* program;
    int* out;
    int oidx;
*/

void init(sienna_assembler_t* assembler, char* prog) {
    assembler->index = 0;
    assembler->program = prog;
    assembler->out = (int*)malloc(strlen(prog)*sizeof(int));
    memset(assembler->out, 0, sizeof(assembler->out));
    assembler->start = 16;
}
void sienna_assembler_assemble(sienna_assembler_t* assembler, char* input_file, char* output_file){
}

/*
#define movir 0xd1
#define movrr 0xd2
#define movrm 0xd3
#define movim 0xd4
#define movmr 0xd5
*/
void mov_impl(sienna_assembler_t* assembler) {
    if(CHECK('i')) {
        STEP;
        if(CHECK('r')){
            PUSH_OUT(movir);
            STEP;
            consume_whitespace(assembler);
            consume_hex(assembler);
            consume_whitespace(assembler);
            consume_comma(assembler);
            consume_whitespace(assembler);
            consume_register(assembler);
        }else if(CHECK('m')) {
            PUSH_OUT(movim);
            STEP;
            consume_whitespace(assembler);
            consume_hex(assembler);
            consume_whitespace(assembler);
            consume_comma(assembler);
            consume_whitespace(assembler);
            consume_address(assembler);
        }
    }else if(CHECK('r')){
        STEP;
        if(CHECK('r')){
            PUSH_OUT(movrr);
            STEP;
            consume_whitespace(assembler);
            consume_register(assembler);
            consume_whitespace(assembler);
            consume_comma(assembler);
            consume_whitespace(assembler);
            consume_register(assembler);
        }
    }else if(CHECK('m')) {
        STEP;
    }else {
        invalid_type(assembler, "i, r, m");
    }
}
void sienna_assembler_assemblestr(sienna_assembler_t* assembler, char* prog){
    init(assembler, prog);
    while(assembler->program[assembler->index] != '\0'){
        if(CHECK_INST("mov")) mov_impl(assembler);
        else { printf("Invalid instruction at index %d, current char '%c'\n", assembler->index, CURRENT); exit(-1);}
        consume_semi(assembler);
        STEP;
    }
    printf("Program: ");
    for(int i = assembler->start; i < assembler->oidx; i++) {
        printf("%04x ", assembler->out[i]);
    }
    printf("\n");
}