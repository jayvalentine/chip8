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
