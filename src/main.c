#include "include/state.h"
#include "include/decode.h"
#include "include/execute.h"
#include "include/platform.h"

#include "stdio.h"
#include "string.h"
#include "time.h"
#include "stdlib.h"

/* Fetches the next instruction from memory,
 * incrementing the program counter to point to
 * the instruction after.
 */
uint16_t fetch(State * s)
{
    uint16_t upper = s->memory[s->pc++] << 8;
    uint16_t opcode = s->memory[s->pc++] | upper;

    return opcode;
}

State s;
Instruction current_instr;

const uint8_t font[5 * 16] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/* 480 instructions per second,
 * so screen updates every 8 instrs
 */
#define SECOND_MS 1000
#define INSTR_DELAY_MS (SECOND_MS / 480)
#define INSTRS_PER_TICK 8 

int main(int argc, char ** argv)
{
    time_t t;
    srand((unsigned) time(&t));

    if (argc != 2)
    {
        puts("Usage: chip8 <program>");
        return 1;
    }

    /* Load font into memory. */
    memcpy(&s.memory[0x50], font, sizeof(font));

    /* Load program from file into memory. */
    uint8_t * m = &s.memory[0x200];
    FILE * f = fopen(argv[1], "rb");
    while (1)
    {
        size_t bytes_read = fread(m, 1, 512, f);
        if (bytes_read != 512) break;
        m += 512;
    }
    fclose(f);

    /* Set entry point of program. */
    s.pc = 0x200;

    platform_init();

    /* Fetch/decode/execute cycle. */
    uint8_t instr_count = 0;
    while (1)
    {
        uint16_t opcode = fetch(&s);
        decode(&current_instr, opcode);

        if (current_instr.opcode == UNKNOWN)
        {
            printf("%04x: Invalid opcode %04x\n", s.pc - 2, opcode);
            break;
        }

        execute(&s, &current_instr);
        instr_count++;
        if (instr_count >= INSTRS_PER_TICK) instr_count = 0;
        
        /* If display memory has been updated, update display. */
        if (instr_count == 0 && s.display_changed)
        {
            platform_update_display(&s);
        }
        
        if (platform_tick()) break;
        platform_delay(INSTR_DELAY_MS);
    }

    platform_quit();
}
