#ifndef _DECODE_H
#define _DECODE_H

#include "stdint.h"

typedef enum _Opcode
{
    CLEAR,
    DRAW,

    JUMP,
    CALL,
    RETURN,

    SKIP_EQ_REG,
    SKIP_NEQ_REG,
    SKIP_EQ_IMM,
    SKIP_NEQ_IMM,

    SET_REG_REG,
    SET_REG_IMM,
    ADD_REG_REG,
    ADD_REG_IMM,

    SUB_XY,
    SUB_YX,

    OR_REG_REG,
    AND_REG_REG,
    XOR_REG_REG,

    SHIFT_R,
    SHIFT_L,

    SET_INDEX_IMM,

    UNKNOWN
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
