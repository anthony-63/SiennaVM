#include "assembler.h"
#include "../vm/instructions.h"
#include "../vm/registers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(X) assembler->program[assembler->index] == X
#define CURRENT assembler->program[assembler->index]
#define IS_HEX (CHECK('0') || CHECK('1') || CHECK('2') || CHECK('3') || CHECK('4') || CHECK('5') || CHECK('6') || CHECK('7') || CHECK('8') || CHECK('9') || \
                CHECK('A') || CHECK('B') || CHECK('C') || CHECK('D') || CHECK('E') || CHECK('F') || CHECK('a') || CHECK('b') || CHECK('c') || CHECK('d') || CHECK('e') || CHECK('f'))
#define STEP assembler->index += 1
#define PUSH_OUT(X) assembler->out[assembler->oidx++] = X
#define CONSUME_COMMAW consume_whitespace(assembler);consume_comma(assembler);consume_whitespace(assembler)
#define STEPW consume_whitespace(assembler);STEP;consume_whitespace(assembler)
#define CHECK_INST(X) ((assembler->program[assembler->index] == X[0]) && (assembler->program[assembler->index + 1] == X[1]) && (assembler->program[assembler->index + 2] == X[2]))
#define CHECK_INST_S2(X) ((assembler->program[assembler->index] == X[0]) && (assembler->program[assembler->index + 1] == X[1]))
#define ICALL(X) STEPW;STEPW;STEPW;X(assembler);
#define ICALL_S2(X) STEPW;STEPW;X(assembler);
#define CONSUME_ADDR_LBL {\
    int tolp = consume_address(assembler); \
    int w = 0;\
    char c = '[';\
    while(c != ']') {\
        c = assembler->program[assembler->index++];\
    }\
} 

char** labelSymbols;
int* labelAddresses;
int lidx = 0;
int address = 0;
#define PUSH_LBL(X, Y) labelSymbols[lidx] = X; labelAddresses[lidx++] = Y;


int find_lbl(sienna_assembler_t* assembler, char* lbl) {
    int found = 0;
    for(int i = 0; i < lidx; i++) {
        if(strcmp(labelSymbols[i], lbl) == 0) {
            return i;
        }
    }
    printf("\nFATAL: Error symbol '%s' not found\n", lbl);
    exit(-1);
} 

void expected(sienna_assembler_t* assembler, char ex) {
    printf("FATAL: Expected a '%c', current char '%c', at index %d\n", ex, CURRENT, assembler->index);
    exit(-1);
}
void invalid_type(sienna_assembler_t* assembler, char* type){
    printf("FATAL: Invalid type '%s', current char '%c', at index %d\n", type, CURRENT, assembler->index);
    exit(-1);
}
void expected_type(sienna_assembler_t* assembler, char* type) {
    printf("FATAL: Expected type '%s', current char '%c', at index %d\n", type, CURRENT, assembler->index);
    exit(-1);
}
void expected_type_but_got(char* type, char* got) {
    printf("FATAL: Expected type '%s' but got '%s'\n", type, got);
    exit(-1);
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
    else if(strcmp(reg, "tr") == 0) return tr;
    else { expected_type_but_got("Register", reg);}
}

void consume_hex(sienna_assembler_t* assembler) {
    if(!CHECK('$')) {
        expected(assembler, '$');
    }
    char* hstr = (char*)malloc(sizeof(char)*0xFFFF);
    memset(hstr, 0, sizeof(hstr));
    STEP;
    int idx = 0;
    while(IS_HEX) {
        hstr[idx++] = assembler->program[assembler->index++];
    }
    PUSH_OUT((int)strtol(hstr, NULL, 16));
    address++;
}
void consume_whitespace(sienna_assembler_t* assembler) {
    while(CHECK(' ') || CHECK('\n')) {
        STEP;
    }
}
int consume_address(sienna_assembler_t* assembler) {
    if(!CHECK('&') && !CHECK('#')) {
        expected(assembler, '&');
        expected(assembler, '#');
    }
    assembler->index--;
    if(CHECK('#')) {
        STEPW;
        STEPW;
        int size = 0;
        if(assembler->program[assembler->index - 1] == '['){
            while(assembler->program[assembler->index + size] != ']') {
                size++;
            }
            char* labelname = (char*)calloc(size, sizeof(char));
            for(int i = 0; i < size; i++) labelname[i] = assembler->program[assembler->index + i];
            int lblidx = find_lbl(assembler, labelname);
            PUSH_OUT(labelAddresses[lblidx]);
            return size;
        }else {
            expected(assembler, '[');
        }
    }
    char* hstr = (char*)malloc(sizeof(char)*0xFFFF);
    memset(hstr, 0, sizeof(hstr));
    STEPW;
    int idx = 0;
    while(IS_HEX) {
        hstr[idx++] = assembler->program[assembler->index++];
    }
    PUSH_OUT((int)strtol(hstr, NULL, 16));
    address++;
    return 0;
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
    PUSH_OUT(get_reg_index(assembler, reg));
    address++;
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
void consume_colon(sienna_assembler_t* assembler){
    if(!CHECK(':')){
        expected(assembler, ':');
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
    labelSymbols = (char**)calloc(0xFFFF, sizeof(char*));
    labelAddresses = (int*)calloc(0xFFFF, sizeof(int));
    if(labelSymbols != NULL) {
        for(int i = 0; i < 0xFFFF; i++) {
            labelSymbols[i] = (char*)calloc(0xFF, sizeof(char));
        }
    }else {
        printf("labelSymbols is NULL??\n");
        exit(0);
    }
    
    assembler->index = 0;
    assembler->ooffs = 18;
    assembler->psize = strlen(prog);
    assembler->out = (int*)malloc(assembler->psize * sizeof(int)*0xFF);
    memset(assembler->out, 0, sizeof(assembler->out));
    assembler->program = prog;
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
            STEPW;
            consume_hex(assembler);
            CONSUME_COMMAW;
            consume_register(assembler);
        }else if(CHECK('m')) {
            PUSH_OUT(movim);
            STEPW;
            consume_hex(assembler);
            CONSUME_COMMAW;
            CONSUME_ADDR_LBL
        }else {
            expected_type(assembler, "r, m");
        }
    }else if(CHECK('r')){
        STEP;
        if(CHECK('r')){
            PUSH_OUT(movrr);
            STEPW;
            consume_register(assembler);
            CONSUME_COMMAW;
            consume_register(assembler);
        }else if(CHECK('m')){
            PUSH_OUT(movrm);
            STEPW;
            consume_register(assembler);
            CONSUME_COMMAW;
            CONSUME_ADDR_LBL
        }else {
            expected_type(assembler, "r, m");
        }
    }else if(CHECK('m')) {
        STEP;
        if(CHECK('r')) {
            PUSH_OUT(movmr);
            STEPW;
            CONSUME_ADDR_LBL
            CONSUME_COMMAW;
            consume_register(assembler);
        }else {
            expected_type(assembler, "r");
        }
    }else {
        expected_type(assembler, "i, r, m");
    }
}

/*
#define addir 0x10
#define addrr 0x11
#define subir 0x12
#define subrr 0x13
#define mulir 0x14
#define mulrr 0x15
*/

void add_impl(sienna_assembler_t* assembler){
    if(CHECK('i')) {
        STEP;
        if(CHECK('r')){
            PUSH_OUT(addir);
            STEPW;
            consume_hex(assembler);
            CONSUME_COMMAW;
            consume_register(assembler);
        }else{
            expected_type(assembler, "r");
        }
    }else if(CHECK('r')) {
        STEP;
        if(CHECK('r')) {
            PUSH_OUT(addrr);
            STEPW;
            consume_register(assembler);
            CONSUME_COMMAW;
            consume_register(assembler);
        }else {
            expected_type(assembler, "r");
        }
    }
    else {
        expected_type(assembler, "i, r");
    }
}

void sub_impl(sienna_assembler_t* assembler){
    if(CHECK('i')) {
        STEP;
        if(CHECK('r')){
            PUSH_OUT(subir);
            STEPW;
            consume_hex(assembler);
            CONSUME_COMMAW;
            consume_register(assembler);
        }else{
            expected_type(assembler, "r");
        }
    }else if(CHECK('r')) {
        STEP;
        if(CHECK('r')) {
            PUSH_OUT(subrr);
            STEPW;
            consume_register(assembler);
            CONSUME_COMMAW;
            consume_register(assembler);
        }else {
            expected_type(assembler, "r");
        }
    }
    else {
        expected_type(assembler, "i, r");
    }
}

void mul_impl(sienna_assembler_t* assembler){
    if(CHECK('i')) {
        STEP;
        if(CHECK('r')){
            PUSH_OUT(mulir);
            STEPW;
            consume_hex(assembler);
            CONSUME_COMMAW;
            consume_register(assembler);
        }else{
            expected_type(assembler, "r");
        }
    }else if(CHECK('r')) {
        STEP;
        if(CHECK('r')) {
            PUSH_OUT(mulrr);
            STEPW;
            consume_register(assembler);
            CONSUME_COMMAW;
            consume_register(assembler);
        }else {
            expected_type(assembler, "r");
        }
    }
    else {
        expected_type(assembler, "i, r");
    }
}
/*
#define incr 0x16
#define decr 0x17
*/
void dec_impl(sienna_assembler_t* assembler){
    if(CHECK('r')){
        PUSH_OUT(decr);
        STEPW;
        consume_register(assembler);
    }else {
        expected_type(assembler, "r");
    }
}
void inc_impl(sienna_assembler_t* assembler){
    if(CHECK('r')){
        PUSH_OUT(incr);
        STEPW;
        consume_register(assembler);
    }else {
        expected_type(assembler, "r");
    }
}
/*
#define cmprr 0x06
#define cmpir 0x07
*/

void cmp_impl(sienna_assembler_t* assembler){
    if(CHECK('r')) {
        STEP;
        if(CHECK('r')){
            PUSH_OUT(cmprr);
            STEPW;
            consume_register(assembler);
            CONSUME_COMMAW;
            consume_register(assembler);
        }else {
            expected_type(assembler, "r");
        }
    }else if(CHECK('i')) {
        STEP;
        if(CHECK('r')){
            PUSH_OUT(cmpir);
            STEPW;
            consume_hex(assembler);
            CONSUME_COMMAW;
            consume_register(assembler);
        }else {
            expected_type(assembler, "r");
        }
    }else {
        expected_type(assembler, "i, r");
    }
}
/*
#define je  0x20
#define jne 0x21
#define jg  0x22
#define jl  0x23
#define jge 0x24
#define jle 0x25
*/
void je_impl(sienna_assembler_t* assembler){
    PUSH_OUT(je);
    STEPW;
    CONSUME_ADDR_LBL
}
void jne_impl(sienna_assembler_t* assembler){
    PUSH_OUT(jne);
    STEPW;
    CONSUME_ADDR_LBL
}
void jg_impl(sienna_assembler_t* assembler){
    PUSH_OUT(jg);
    STEPW;
    CONSUME_ADDR_LBL
}
void jl_impl(sienna_assembler_t* assembler){
    PUSH_OUT(jl);
    STEPW;
    CONSUME_ADDR_LBL
}
void jge_impl(sienna_assembler_t* assembler){
    PUSH_OUT(jge);
    STEPW;
    CONSUME_ADDR_LBL
}
void jle_impl(sienna_assembler_t* assembler){
    PUSH_OUT(jle);
    STEPW;
    CONSUME_ADDR_LBL
}
/*
#define pushr 0x30
#define pushi 0x31
#define popd  0x32
#define popr  0x33
*/

void impl_psh(sienna_assembler_t* assembler) {
    if(CHECK('r')) {
        PUSH_OUT(pushr);
        STEPW;
        consume_register(assembler);
    }else if(CHECK('i')) {
        PUSH_OUT(pushi);
        STEPW;
        consume_hex(assembler);
    }else {
        expected_type(assembler, "r, i");
    }
} 
void impl_pop(sienna_assembler_t* assembler) {
    if(CHECK('r')) {
        PUSH_OUT(popr);
        STEPW;
        consume_register(assembler);
    }else {
        expected_type(assembler, "r");
    }
}
/*
#define mbir 0xf0
*/
void impl_mbir(sienna_assembler_t* assembler) {
    PUSH_OUT(mbir);
    STEPW;
    consume_hex(assembler);
}
/*
#define nop 0x00
*/
void impl_nop(sienna_assembler_t* assembler){
    PUSH_OUT(nop);
}

int impl_mkl(sienna_assembler_t* assembler){
    STEPW;
    int size = 0;
    while(assembler->program[assembler->index + size] != ':') {
        size++;
    }
    char* labelname = (char*)calloc(size, sizeof(char));
    for(int i = 0; i < size; i++) labelname[i] = assembler->program[assembler->index + i];
    printf("\nlabelname: %s, size: %d, address_pointer: %d\n", labelname, size, address + 1);
    PUSH_LBL(labelname, address);
    return size;
}
int skip_lbl(sienna_assembler_t* assembler) {
    STEPW;
    int i = 0;
    while(assembler->program[assembler->index + i] != ':') { 
        i++;
    }
    return i;
}
void stage_1(sienna_assembler_t* assembler) {
    while(assembler->index < assembler->psize){
        if(     CHECK_INST   ("mov")) { address++; address++; address++; STEPW; STEPW; STEPW; }
        else if(CHECK_INST   ("add")) { address++; address++; address++; STEPW; STEPW; STEPW; }
        else if(CHECK_INST   ("sub")) { address++; address++; address++; STEPW; STEPW; STEPW; }
        else if(CHECK_INST   ("mul")) { address++; address++; address++; STEPW; STEPW; STEPW; }
        else if(CHECK_INST   ("inc")) { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST   ("dec")) { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST   ("cmp")) { address++; address++; address++; STEPW; STEPW; STEPW; }
        else if(CHECK_INST_S2("je"))  { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST_S2("jl"))  { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST_S2("jg"))  { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST   ("jne")) { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST   ("jle")) { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST   ("jge")) { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST   ("psh")) { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST   ("pop")) { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST   ("mbr")) { address++; address++; STEPW; STEPW; }
        else if(CHECK_INST   ("nop")) { address++; STEPW;}
        else if(CURRENT == '+'){
            int toloop = impl_mkl(assembler);
            for(int i = 0; i < toloop; i++) {
                STEPW;
            }
        }else {
            STEP;
        }
    }
    assembler->index = 0;
    address = 0;
}

void print_line(sienna_assembler_t* assembler) {
    int start = 0;
    int end = 0;
    while(assembler->program[assembler->index + start] != '\n') start--;
    while(assembler->program[assembler->index + end  ] != '\n') end++;
    char* line = (char*)calloc(end - start, sizeof(char));
    for(int i = start + 1; i < end + 1; i++) {
        printf("%c", assembler->program[assembler->index + i]);
    }
}

int asm_loop(sienna_assembler_t* assembler) {
    stage_1(assembler);
    while(assembler->index < assembler->psize){
        if(     CHECK_INST   ("mov")) { ICALL   (mov_impl) }
        else if(CHECK_INST   ("add")) { ICALL   (add_impl) }
        else if(CHECK_INST   ("sub")) { ICALL   (sub_impl) }
        else if(CHECK_INST   ("mul")) { ICALL   (mul_impl) }
        else if(CHECK_INST   ("inc")) { ICALL   (inc_impl) }
        else if(CHECK_INST   ("dec")) { ICALL   (dec_impl) }
        else if(CHECK_INST   ("cmp")) { ICALL   (cmp_impl) }
        else if(CHECK_INST_S2("je"))  { ICALL_S2(je_impl ) }
        else if(CHECK_INST_S2("jl"))  { ICALL_S2(jl_impl ) }
        else if(CHECK_INST_S2("jg"))  { ICALL_S2(jg_impl ) }
        else if(CHECK_INST   ("jne")) { ICALL   (jne_impl) }
        else if(CHECK_INST   ("jle")) { ICALL   (jle_impl) }
        else if(CHECK_INST   ("jge")) { ICALL   (jge_impl) }
        else if(CHECK_INST   ("psh")) { ICALL   (impl_psh) }
        else if(CHECK_INST   ("pop")) { ICALL   (impl_pop) }
        else if(CHECK_INST   ("mbr")) { ICALL   (impl_mbir)}
        else if(CHECK_INST   ("nop")) { ICALL   (impl_nop) }
        else if(CURRENT == '+') { 
            int toloop = skip_lbl(assembler);
            for(int i = 0; i < toloop; i++) {
                STEPW;
            }
        }
        else { 
            char inst[4] = {assembler->program[assembler->index - 1], assembler->program[assembler->index], assembler->program[assembler->index + 1]};
            printf("FATAL: Invalid instruction '%s' at index %d, current char '%c'\nLine: ", inst, assembler->index, CURRENT);
            print_line(assembler);
            printf("\n");
            return 0xefff;
        }
        consume_whitespace(assembler);
        consume_semi(assembler);
        STEPW;
        consume_whitespace(assembler);
    }
    PUSH_OUT(hlt);
    return 0;
}
void sienna_assembler_assemble(sienna_assembler_t* assembler, char* input_file, char* output_file){
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen (input_file , "rb" );
    if( !fp ) perror(input_file),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );

    rewind( fp );
    
    buffer = calloc( 1, lSize+1 );
    
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);
    if( 1!=fread( buffer , lSize, 1 , fp) )
    
    fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
    fclose(fp);

    printf("Program from file:\n%s\n", buffer);
    init(assembler, buffer);
    int err = asm_loop(assembler);
    if(err == 0xefff) { 
        remove(output_file);
        exit(-1);
    }
    FILE* ofile = fopen(output_file, "wb");
    printf("Program: ");
    int sizeofprog = assembler->oidx - assembler->ooffs;
    fwrite((const void*)&sizeofprog, sizeof(int), 1, ofile);
    printf("%04x ", sizeofprog);
    for(int i = assembler->ooffs; i < assembler->oidx; i++) {
        int tmp = assembler->out[i];
        fwrite((const void*)&tmp, sizeof(int), 1, ofile);
        printf("%04x ", tmp);
    }
    printf("\n");
    free(buffer);
    
}

void sienna_assembler_assemblestr(sienna_assembler_t* assembler, char* prog){
    init(assembler, prog);
    asm_loop(assembler);
    printf("Program: ");
    for(int i = assembler->ooffs; i < assembler->oidx; i++) {
        printf("%04x ", assembler->out[i]);
    }
    printf("\n");
}

void sienna_assembler_cleanup(sienna_assembler_t* assembler){
    free(assembler->out);
    free(labelSymbols);
}