#include "include/execute.h"

#include "string.h"

/* Clears all bits in the display. */
void exec_clear_screen(State * s)
{
    /* Clear the display. */
    memset(s->display, 0, sizeof(s->display));

    /* Mark the display as having changed. */
    s->display_changed = true;
}

void exec_draw(State * s, uint8_t x_reg, uint8_t y_reg, uint8_t n)
{
    /* Sprite starts at the address pointed to by index register. */
    address addr = s->i;

    uint8_t x = s->registers[x_reg] % DISPLAY_WIDTH;
    uint8_t y = s->registers[y_reg] % DISPLAY_HEIGHT;

    uint8_t x_in_bytes = x / 8;

    /* Calculate how many bits spill over to the next column (if any) */
    uint8_t spillover = x % 8;

    /* Draw the sprite. */
    while (n > 0)
    {
        s->display[y][x_in_bytes] = s->memory[addr] >> spillover;

        /* If the X coordinate isn't perfectly aligned, spill over
         * to the next column. */
        if (spillover > 0 && x_in_bytes != ((DISPLAY_WIDTH / 8) - 1))
        {
            s->display[y][x_in_bytes+1] = s->memory[addr] << (8-spillover);
        }

        y++;

        if (y == DISPLAY_HEIGHT) break;

        addr++;
        n--;
    }
}

/* Jumps to the given destination. */
void exec_jump(State * s, address destination)
{
    s->pc = destination;
}

/* Sets register to immediate value. */
void exec_set_reg_imm(State *s, uint8_t reg, uint8_t imm)
{
    s->registers[reg] = imm;
}

/* Adds immediate value to register. */
void exec_add_reg_imm(State *s, uint8_t reg, uint8_t imm)
{
    s->registers[reg] += imm;
}

/* Sets index register. */
void exec_set_index_imm(State *s, uint16_t imm)
{
    s->i = imm;
}
