#include "include/decode.h"

#define EXTRACT_XNN(i, o)     \
    i->X = (o >> 8) & 0x000f; \
    i->NN = o & 0x00ff

#define EXTRACT_NNN(i, o)     \
    i->NNN = o & 0x0fff

#define EXTRACT_XYN(i, o)     \
    i->X = (o >> 8) & 0x000f; \
    i->Y = (o >> 4) & 0x000f; \
    i->N = o & 0xf;

#define EXTRACT_XY(i, o)      \
    i->X = (o >> 8) & 0x000f; \
    i->Y = (o >> 4) & 0x000f;

void decode(Instruction * instr, uint16_t opcode)
{
    instr->opcode = UNKNOWN;

    uint16_t opcode_nibble = opcode >> 12;

    switch (opcode_nibble)
    {
        case 0x0:
            switch (opcode)
            {
                case 0x00e0:
                    instr->opcode = CLEAR;
                    break;
            }
            break;
        case 0x1:
            instr->opcode = JUMP;
            EXTRACT_NNN(instr, opcode);
            break;
        case 0x3:
            instr->opcode = SKIP_EQ_IMM;
            EXTRACT_XNN(instr, opcode);
            break;
        case 0x4:
            instr->opcode = SKIP_NEQ_IMM;
            EXTRACT_XNN(instr, opcode);
            break;
        case 0x5:
            instr->opcode = SKIP_EQ_REG;
            EXTRACT_XY(instr, opcode);
            break;
        case 0x6:
            instr->opcode = SET_REG_IMM;
            EXTRACT_XNN(instr, opcode);
            break;
        case 0x7:
            instr->opcode = ADD_REG_IMM;
            EXTRACT_XNN(instr, opcode);
            break;
        case 0x9:
            instr->opcode = SKIP_NEQ_REG;
            EXTRACT_XY(instr, opcode);
            break;
        case 0xA:
            instr->opcode = SET_INDEX_IMM;
            EXTRACT_NNN(instr, opcode);
            break;
        case 0xD:
            instr->opcode = DRAW;
            EXTRACT_XYN(instr, opcode);
            break;
    }
}
