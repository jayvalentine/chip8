#ifndef _EXECUTE_H
#define _EXECUTE_H

#include "include/state.h"

void exec_clear_screen(State * s);
void exec_jump(State * s, uint16_t destination);

#endif /* _EXECUTE_H */
