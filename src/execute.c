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
