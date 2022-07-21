#include "include/state.h"
#include "include/decode.h"
#include "include/execute.h"

#include "stdio.h"

uint16_t fetch(State * s)
{
    uint16_t upper = s->memory[s->pc++] << 8;
    uint16_t opcode = s->memory[s->pc++] | upper;

    return opcode;
}

State s;
Instruction current_instr;

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        puts("Usage: chip8 <program>");
        return 1;
    }

    s.pc = 0x200;

    while (1)
    {
        uint16_t opcode = fetch(&s);
        decode(&current_instr, opcode);

        if (current_instr.opcode == UNKNOWN)
        {
            printf("%04x: Invalid opcode %04x\n", s.pc - 2, opcode);
            return 1;
        }

        execute(&s, &current_instr);
    }
}
