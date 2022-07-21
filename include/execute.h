#ifndef _EXECUTE_H
#define _EXECUTE_H

#include "include/state.h"

/* Display control instructions. */

void exec_clear_screen(State * s);
void exec_draw(State * s, uint8_t x_reg, uint8_t y_reg, uint8_t n);

/* Control flow instructions. */

void exec_jump(State * s, uint16_t destination);

/* Arithmetic/logic instructions. */

void exec_set_reg_imm(State *s, uint8_t reg, uint8_t imm);
void exec_add_reg_imm(State *s, uint8_t reg, uint8_t imm);

void exec_set_index_imm(State *s, uint16_t imm);

#endif /* _EXECUTE_H */
