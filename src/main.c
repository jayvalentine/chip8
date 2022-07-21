#include "include/state.h"
#include "include/decode.h"
#include "include/execute.h"

#include "stdio.h"
#include "string.h"

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

void display_update(State * s)
{
    printf("\033[1;1H");
    
    /* Reset to top-right corner. */
    /* Iterate over each row in the display. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        /* Iterate over each byte in the row. */
        for (int x_in_bytes = 0; x_in_bytes < DISPLAY_WIDTH / 8; x_in_bytes++)
        {
            uint8_t b = s->display[y][x_in_bytes];
            for (int i = 0; i < 8; i++)
            {
                char c;
                if (b & 0b10000000)
                {
                    c = '#';
                }
                else
                {
                    c = ' ';
                }
                putchar(c);
                b <<= 1;
            }
        }
        printf("\n");
    }
}

int main(int argc, char ** argv)
{
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

    printf("\033[2J");

    /* Fetch/decode/execute cycle. */
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

        /* If screen has been updated, print to stdout. */
        if (s.display_changed)
        {
            display_update(&s);
        }
    }
}
