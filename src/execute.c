#include "include/execute.h"

#include "string.h"

/* Type declaration for a helper function for executing an instruction.
 */
typedef void (*execute_helper)(State *, Instruction *);

/* Clears all bits in the display. */
static void exec_clear_screen(State * s, Instruction * i)
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
static void set_display(State * s, uint8_t x_in_bytes, uint8_t y, uint8_t mask)
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

static void exec_draw(State * s, Instruction * i)
{
    uint8_t n = i->N;

    /* Sprite starts at the address pointed to by index register. */
    address addr = s->i;

    uint8_t x = s->registers[i->X] % DISPLAY_WIDTH;
    uint8_t y = s->registers[i->Y] % DISPLAY_HEIGHT;

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

    /* Mark display as having changed. */
    s->display_changed = true;
}

/* Jumps to the given destination. */
static void exec_jump(State * s, Instruction * i)
{
    s->pc = i->NNN;
}

/* Calls the subroutine at the given destination. */
static void exec_subroutine_call(State * s, Instruction * i)
{
    /* Save current PC on the stack. This will be the address
     * of the instruction after the call.
     */
    s->stack.values[s->stack.size] = s->pc;
    s->stack.size++;

    /* Transfer control to destination address. */
    s->pc = i->NNN;
}

/* Returns from the current subroutine. */
static void exec_subroutine_return(State * s, Instruction * i)
{
    /* Set PC to top of stack. */
    s->stack.size--;
    s->pc = s->stack.values[s->stack.size];
}

/* Skips next instruction if VX == VY */
static void exec_skip_eq_reg(State * s, Instruction * i)
{
    s->skip_next = (s->registers[i->X] == s->registers[i->Y]);
}

/* Skips next instruction if VX == IMM */
static void exec_skip_eq_imm(State * s, Instruction * i)
{
    s->skip_next = (s->registers[i->X] == i->NN);
}

/* Skips next instruction if VX != VY */
static void exec_skip_neq_reg(State * s, Instruction * i)
{
    s->skip_next = (s->registers[i->X] != s->registers[i->Y]);
}

/* Skips next instruction if VX != IMM */
static void exec_skip_neq_imm(State * s, Instruction * i)
{
    s->skip_next = (s->registers[i->X] != i->NN);
}

/* Sets register to immediate value. */
static void exec_set_reg_imm(State *s, Instruction * i)
{
    s->registers[i->X] = i->NN;
}

/* Adds immediate value to register. */
static void exec_add_reg_imm(State *s, Instruction * i)
{
    s->registers[i->X] += i->NN;
}

/* Sets index register. */
static void exec_set_index_imm(State *s, Instruction * i)
{
    s->i = i->NNN;
}

static void invalid_instruction(State * s, Instruction * i)
{
    /* Do nothing. */
}

/* Lookup table for helper functions for each instruction.
 * Given the opcode of an instruction we can execute it by
 * calling the corresponding function in this table.
 */
const execute_helper execute_lut[] =
{
    exec_clear_screen,
    exec_draw,

    exec_jump,
    exec_subroutine_call,
    exec_subroutine_return,

    exec_skip_eq_reg,
    exec_skip_neq_reg,
    exec_skip_eq_imm,
    exec_skip_neq_imm,

    exec_set_reg_imm,
    exec_add_reg_imm,

    exec_set_index_imm,

    invalid_instruction
};

/* Entry point for execution. Determines the type of instruction
 * and calls the relevant execution function.
 */
void execute(State * s, Instruction * i)
{
    execute_lut[i->opcode](s, i);
}
