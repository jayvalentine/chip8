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

/* Jumps to the given destination. */
void exec_jump(State * s, address destination)
{
    s->pc = destination;
}

/* Sets register to immediate value. */
void exec_set_reg_imm(State *s, uint8_t reg, uint8_t imm)
{
    
}

/* Adds immediate value to register. */
void exec_add_reg_imm(State *s, uint8_t reg, uint8_t imm)
{

}
