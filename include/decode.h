#ifndef _DECODE_H
#define _DECODE_H

#include "stdint.h"

typedef enum _Opcode
{
    CLEAR,
    DRAW,

    JUMP,

    SET_REG_IMM,
    ADD_REG_IMM,

    SET_INDEX_IMM
} Opcode;

typedef struct _Instruction
{
    Opcode opcode;
    uint16_t NNN;
    uint8_t NN;
    uint8_t N;
    uint8_t X;
    uint8_t Y;
} Instruction;

void decode(Instruction * instr, uint16_t opcode);

#endif /* _DECODE_H */
