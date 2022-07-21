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

/* Helper function to set display.
 *
 * s: state pointer
 * x_in_bytes: index into row in bytes
 * y: row index
 * mask: pattern for setting bits of display.
 */
void set_display(State * s, uint8_t x_in_bytes, uint8_t y, uint8_t mask)
{
    uint8_t * display_byte_ptr = &s->display[y][x_in_bytes];

    uint8_t current = *display_byte_ptr;

    /* A pixel will be switched off if any bits are set in both
     * the current value and the mask.
     */
    if (current & mask) s->registers[0xf] = 1;

    /* XOR the current value with the mask and write back
     * to display memory.
     */
    *display_byte_ptr = current ^ mask;
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
        uint8_t mask = s->memory[addr];

        set_display(s, x_in_bytes, y, mask >> spillover);

        /* If the X coordinate isn't perfectly aligned, spill over
         * to the next column. */
        if (spillover > 0 && x_in_bytes != ((DISPLAY_WIDTH / 8) - 1))
        {
            set_display(s, x_in_bytes + 1, y, mask << (8-spillover));
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
