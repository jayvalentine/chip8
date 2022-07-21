#include "include/execute.h"

#include "string.h"

void exec_clear_screen(State * s)
{
    /* Clear the display. */
    memset(s->display, 0, DISPLAY_SIZE);

    /* Mark the display as having changed. */
    s->display_changed = true;
}
