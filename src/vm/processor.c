#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "processor.h"
#include "instructions.h"
//macros to make this easier
#define GET_REG(X, Y) sienna_registers_getregister(&(X->registers), Y)
#define SET_REG(X, Y, Z) sienna_registers_setregister(&(X->registers), Y, Z)

void step(sienna_processor_t* processor) {
   SET_REG(processor, ip, GET_REG(processor, ip) + 1);
}

int fetch(sienna_processor_t* processor) {
    int t = sienna_bankeddatabus_fetch(&processor->databus, GET_REG(processor, mb), GET_REG(processor, ip));
    step(processor);
    return t;
}

int impl_movir(sienna_processor_t* processor) {
    int imm = fetch(processor);
    int reg = fetch(processor);
    SET_REG(processor, reg, imm);
    return 0x00;
}
int impl_movrr(sienna_processor_t* processor){
    int reg = fetch(processor);
    int regt = fetch(processor);
    SET_REG(processor, regt, GET_REG(processor, reg));
    return 0x00;
}
int impl_movrm(sienna_processor_t* processor){
    int data = GET_REG(processor, fetch(processor));
    int addr = fetch(processor);
    sienna_bankeddatabus_send(&processor->databus, GET_REG(processor, mb), data, addr);
    return 0x00;
}
int impl_movim(sienna_processor_t* processor){
    int imm = fetch(processor);
    int addr = fetch(processor);
    sienna_bankeddatabus_send(&processor->databus, GET_REG(processor, mb), imm, addr);
    return 0x00;
}
int impl_movmr(sienna_processor_t* processor){
    int addr = fetch(processor);
    int reg = fetch(processor);
    SET_REG(processor, reg, sienna_bankeddatabus_fetch(&processor->databus, GET_REG(processor, mb), addr));
    return 0x00;
}
int impl_addir(sienna_processor_t* processor){
    int imm = fetch(processor);
    int regd = GET_REG(processor, fetch(processor));
    SET_REG(processor, zr, imm + regd);
    return 0x00;
}
int impl_addrr(sienna_processor_t* processor) {
    int regf1 = GET_REG(processor, fetch(processor));
    int regf2 = GET_REG(processor, fetch(processor));
    SET_REG(processor, zr, regf1 + regf2);
    return 0x00;
}
int impl_subir(sienna_processor_t* processor){
    int imm = fetch(processor);
    int regd = GET_REG(processor, fetch(processor));
    SET_REG(processor, zr, regd - imm);
    return 0x00;
}
int impl_subrr(sienna_processor_t* processor){
    int regf1 = GET_REG(processor, fetch(processor));
    int regf2 = GET_REG(processor, fetch(processor));
    SET_REG(processor, zr, regf1 - regf2);
    return 0x00;
}
int impl_mulir(sienna_processor_t* processor){
    int imm = fetch(processor);
    int regd = GET_REG(processor, fetch(processor));
    SET_REG(processor, zr, imm * regd);
    return 0x00;
}
int impl_mulrr(sienna_processor_t* processor){
    int regf1 = GET_REG(processor, fetch(processor));
    int regf2 = GET_REG(processor, fetch(processor));
    SET_REG(processor, zr, regf1 * regf2);
    return 0x00;
}
int impl_inc(sienna_processor_t* processor){
    int reg = fetch(processor);
    SET_REG(processor, reg, GET_REG(processor, reg) + 1);
    return 0x00;
}
int impl_dec(sienna_processor_t* processor){
    int reg = fetch(processor);
    SET_REG(processor, reg, GET_REG(processor, reg) - 1);
    return 0x00;
}
int impl_cmprr(sienna_processor_t* processor){
    int valtcf = GET_REG(processor, fetch(processor));
    int valtcs = GET_REG(processor, fetch(processor));
    SET_REG(processor, jr, 0);
    // 0x[0][0][0][0][0][0]
    //   EQ NE  G  L GE LE
    if(valtcf == valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x100000);
    if(valtcf != valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x010000);
    if(valtcf >  valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x001000);
    if(valtcf <  valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x000100);
    if(valtcf >= valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x000010);
    if(valtcf <= valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x000001);
    return 0x00;
}
int impl_cmpir(sienna_processor_t* processor){
    int valtcf = fetch(processor);
    int valtcs = GET_REG(processor, fetch(processor));
    SET_REG(processor, jr, 0);
    // 0x[0][0][0][0][0][0]
    //   EQ NE  G  L GE LE
    if(valtcf == valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x100000);
    if(valtcf != valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x010000);
    if(valtcf >  valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x001000);
    if(valtcf <  valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x000100);
    if(valtcf >= valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x000010);
    if(valtcf <= valtcs) SET_REG(processor, jr, GET_REG(processor, jr) | 0x000001);
    return 0x00;
}
int impl_je (sienna_processor_t* processor){
    int addr = fetch(processor);
    if((GET_REG(processor, jr) & 0xF00000) == 0x100000) SET_REG(processor, ip, addr);
    return 0x00;
}
int impl_jne(sienna_processor_t* processor){
    int addr = fetch(processor);
    if((GET_REG(processor, jr) & 0x0F0000) == 0x010000) SET_REG(processor, ip, addr);
    return 0x00;
}
int impl_jg (sienna_processor_t* processor){
    int addr = fetch(processor);
    if((GET_REG(processor, jr) & 0x00F000) == 0x001000) SET_REG(processor, ip, addr);
    return 0x00;
}
int impl_jl (sienna_processor_t* processor){
    int addr = fetch(processor);
    if((GET_REG(processor, jr) & 0x000F00) == 0x000100) SET_REG(processor, ip, addr);
    return 0x00;
}
int impl_jge(sienna_processor_t* processor){
    int addr = fetch(processor);
    if((GET_REG(processor, jr) & 0x0000F0) == 0x000010) SET_REG(processor, ip, addr);
    return 0x00;
}
int impl_jle(sienna_processor_t* processor){
    int addr = fetch(processor);
    if((GET_REG(processor, jr) & 0x00000F) == 0x000001) SET_REG(processor, ip, addr);
    return 0x00;
}
int impl_pushr(sienna_processor_t* processor) {
    int r = fetch(processor);
    sienna_stack_push(processor->stack, GET_REG(processor, r));
    return 0x00;
}
int impl_pushi(sienna_processor_t* processor) {
    int imm = fetch(processor);
    sienna_stack_push(processor->stack, imm);
    return 0x00;
}
int impl_popd(sienna_processor_t* processor) {
    sienna_stack_pop(processor->stack);
    return 0x00;
}
int impl_popr(sienna_processor_t* processor) {
    int r = fetch(processor);
    SET_REG(processor, r, sienna_stack_pop(processor->stack));
    return 0x00;
}
int impl_mbir(sienna_processor_t* processor) {
    int val = fetch(processor);
    SET_REG(processor, mb, val);
    SET_REG(processor, ip, 0x00);
    return 0x00;
}
int impl_nop(sienna_processor_t* processor) {
    return 0x00;
}

int execute(sienna_processor_t* processor, int instruction){
    SET_REG(processor, tr, 0);
    switch(instruction){
        case hlt:   return 0x60;
        case movir: return impl_movir(processor);
        case movrr: return impl_movrr(processor);
        case movrm: return impl_movrm(processor);
        case movim: return impl_movim(processor);
        case movmr: return impl_movmr(processor);
        case addir: return impl_addir(processor);
        case addrr: return impl_addrr(processor);
        case subir: return impl_subir(processor);
        case subrr: return impl_subrr(processor);
        case mulir: return impl_mulir(processor);
        case mulrr: return impl_mulrr(processor);
        case incr: return impl_inc(processor);
        case decr: return impl_dec(processor);
        case cmprr: return impl_cmprr(processor);
        case cmpir: return impl_cmpir(processor);
        case je : return impl_je (processor);
        case jne: return impl_jne(processor);
        case jg : return impl_jg (processor);
        case jl : return impl_jl (processor);
        case jge: return impl_jge(processor);
        case jle: return impl_jle(processor);
        case pushr: return impl_pushr(processor);
        case pushi: return impl_pushi(processor);
        case popd : return impl_popd(processor);
        case popr : return impl_popr(processor);
        case mbir: return impl_mbir(processor);
        case nop: return impl_nop(processor);
        default: return 0xe5;
    }
}


void error_message(int code, char* fmt, ...){
    printf("\n%02x: ", code);
    va_list va;
    va_start(va, fmt);
    vprintf(fmt, va);
    va_end(va);
    printf("\n");
    exit(-1);
}

void dbg(sienna_processor_t* processor) {
    printf("\nip: %04x\n", GET_REG(processor, ip));
    printf("mb: %04x\n", GET_REG(processor, mb));
    printf("ar: %04x\n", GET_REG(processor, ar));
    printf("br: %04x\n", GET_REG(processor, br));
    printf("cr: %04x\n", GET_REG(processor, cr));
    printf("dr: %04x\n", GET_REG(processor, dr));
    printf("er: %04x\n", GET_REG(processor, er));
    printf("fr: %04x\n", GET_REG(processor, fr));
    printf("gr: %04x\n", GET_REG(processor, gr));
    printf("hr: %04x\n", GET_REG(processor, hr));
    printf("zr: %04x\n", GET_REG(processor, zr));
    printf("tr: %01x\n", GET_REG(processor, tr));
    printf("jr: %06x\n", GET_REG(processor, jr));
    
    printf("%04x: ", GET_REG(processor, ip));
    for(int i = 0; i < 10; i++){
        printf("%02x ", sienna_bankeddatabus_fetch(&processor->databus, GET_REG(processor, mb), GET_REG(processor, ip) + i));
    }
}

void herror(sienna_processor_t* processor, int code){
    switch(code){
        case 0x60: exit(0);
        case 0x00: return;
        case 0xe5: error_message(code, "Instruction %02x not found!\n", sienna_bankeddatabus_fetch(&processor->databus, GET_REG(processor, mb), GET_REG(processor, ip)));
        default: error_message(code, "Invalid error code\n");
    }
}

void sienna_processor_run(sienna_processor_t* processor, int location, int debug){
    sienna_registers_resetflags(&processor->registers);
    if(debug) dbg(processor);
    SET_REG(processor, ip, location);
    while(1){
        herror(processor, execute(processor, fetch(processor)));
        if(debug) dbg(processor);
    }
}

void sienna_processor_load(sienna_processor_t* processor, int* program, int psize, int bank, int location){
    printf("Loading program: \n");
    for(int i = 0; i < psize; i++){
        sienna_bankeddatabus_send(&processor->databus, bank, program[i], i + location);
        printf("%06x <- %02x -> (%02x)\n", i + location, program[i], sienna_bankeddatabus_fetch(&processor->databus, bank, i + location));
    }
    printf("\n");
}
void sienna_processor_loadbin(sienna_processor_t* processor, char* bin_file, int bank, int location){
    FILE* bfile = fopen(bin_file, "rb");
    if(!bfile) perror(bin_file),exit(-1);
    int size = 0;
    fread(&size, sizeof(int), 1, bfile);
    int* prog = (int*)malloc(sizeof(int)*size);
    memset(prog, 0, sizeof(prog));
    int t = 0;
    for(int i = 0; i < size; i++) {
        fread(&t, sizeof(int), 1, bfile);
        prog[i] = t;
    }
    sienna_processor_load(processor, prog, size, bank, location);
}
void sienna_processor_init(sienna_processor_t* processor, int bank_count){
    sienna_bankeddatabus_init(&processor->databus, bank_count);
    sienna_registers_init(&processor->registers);
    sienna_stack_init(&processor->stack, 0xFFFF);
    SET_REG(processor, ip, 0);
    SET_REG(processor, mb, 0);
    SET_REG(processor, ar, 0);
    SET_REG(processor, br, 0);
    SET_REG(processor, cr, 0);
    SET_REG(processor, dr, 0);
    SET_REG(processor, er, 0);
    SET_REG(processor, fr, 0);
    SET_REG(processor, gr, 0);
    SET_REG(processor, hr, 0);
    SET_REG(processor, zr, 0);
    SET_REG(processor, jr, 0);
}