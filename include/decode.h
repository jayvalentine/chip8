#ifndef _DECODE_H
#define _DECODE_H

#include "stdint.h"

typedef enum _Opcode
{
    CLEAR,
    JUMP
} Opcode;

typedef struct _Instruction
{
    Opcode opcode;
    uint16_t NNN;
} Instruction;

void decode(Instruction * instr, uint16_t opcode);

#endif /* _DECODE_H */
