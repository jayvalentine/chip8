#include "include/decode.h"

void decode(Instruction * instr, uint16_t opcode)
{
    uint8_t opcode_nibble = opcode >> 12;

    switch (opcode_nibble)
    {
        case 0x0:
            instr->opcode = CLEAR;
            break;
        case 0x1:
            instr->opcode = JUMP;
            instr->NNN = opcode & 0x0fff;
            break;
    }
}
