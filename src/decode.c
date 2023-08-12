#include "include/decode.h"

#define EXTRACT_XNN(i, o)     \
    i->X = (o >> 8) & 0x000f; \
    i->NN = o & 0x00ff

#define EXTRACT_NNN(i, o)     \
    i->NNN = o & 0x0fff

#define EXTRACT_XYN(i, o)     \
    i->X = (o >> 8) & 0x000f; \
    i->Y = (o >> 4) & 0x000f; \
    i->N = o & 0xf

#define EXTRACT_XY(i, o)      \
    i->X = (o >> 8) & 0x000f; \
    i->Y = (o >> 4) & 0x000f

#define EXTRACT_X(i, o)       \
    i->X = (o >> 8) & 0x000f

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
                case 0x00ee:
                    instr->opcode = RETURN;
                    break;
            }
            break;
        case 0x1:
            instr->opcode = JUMP;
            EXTRACT_NNN(instr, opcode);
            break;
        case 0x2:
            instr->opcode = CALL;
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
        case 0x8:
            EXTRACT_XY(instr, opcode);
            switch (opcode & 0x000f)
            {
                case 0x0:
                    instr->opcode = SET_REG_REG;
                    break;
                case 0x1:
                    instr->opcode = OR_REG_REG;
                    break;
                case 0x2:
                    instr->opcode = AND_REG_REG;
                    break;
                case 0x3:
                    instr->opcode = XOR_REG_REG;
                    break;
                case 0x4:
                    instr->opcode = ADD_REG_REG;
                    break;
                case 0x5:
                    instr->opcode = SUB_XY;
                    break;
                case 0x6:
                    instr->opcode = SHIFT_R;
                    break;
                case 0x7:
                    instr->opcode = SUB_YX;
                    break;
                case 0xe:
                    instr->opcode = SHIFT_L;
                    break;
            }
            break;
        case 0x9:
            instr->opcode = SKIP_NEQ_REG;
            EXTRACT_XY(instr, opcode);
            break;
        case 0xA:
            instr->opcode = SET_INDEX_IMM;
            EXTRACT_NNN(instr, opcode);
            break;
        case 0xC:
            instr->opcode = RANDOM;
            EXTRACT_XNN(instr, opcode);
            break;
        case 0xD:
            instr->opcode = DRAW;
            EXTRACT_XYN(instr, opcode);
            break;
        case 0xF:
            EXTRACT_X(instr, opcode);
            switch (opcode & 0xff)
            {
                case 0x07:
                    instr->opcode = TIMER_GET_DELAY;
                    break;
                case 0x15:
                    instr->opcode = TIMER_SET_DELAY;
                    break;
                case 0x18:
                    instr->opcode = TIMER_SET_SOUND;
                    break;
                case 0x1e:
                    instr->opcode = ADD_INDEX;
                    break;
                case 0x29:
                    instr->opcode = FONT_CHAR;
                    break;
                case 0x33:
                    instr->opcode = BCD_CONVERT;
                    break;
                case 0x55:
                    instr->opcode = STORE;
                    break;
                case 0x65:
                    instr->opcode = LOAD;
                    break;
            }
    }
}
