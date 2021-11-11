#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#define hlt 0xff

#define movir 0xd1
#define movrr 0xd2
#define movrm 0xd3
#define movim 0xd4
#define movmr 0xd5

#define addir 0x10
#define addrr 0x11
#define subir 0x12
#define subrr 0x13
#define mulir 0x14
#define mulrr 0x15

#define incr 0x16
#define decr 0x17

#define cmprr 0x06
#define cmpir 0x07

#define je  0x20
#define jne 0x21
#define jg  0x22
#define jl  0x23
#define jge 0x24
#define jle 0x25

#define pushr 0x30
#define pushi 0x31
#define popd  0x32
#define popr  0x33

#define mbir 0xf0

#define nop 0x00
#endif
