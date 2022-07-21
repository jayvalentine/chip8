#ifndef _STATE_H
#define _STATE_H

#include "stdbool.h"
#include "stdint.h"

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE (DISPLAY_HEIGHT * (DISPLAY_WIDTH / 8))

typedef uint16_t address;

#define STACK_LIMIT 16

typedef struct _Stack
{
    address values[STACK_LIMIT];
    uint8_t size;
} Stack;

/* Represents the emulator state. */
typedef struct _State
{
    address pc;

    /* Registers 0 through F. */
    uint8_t registers[16];

    /* Index register. */
    uint16_t i;

    /* General purpose memory. */
    uint8_t memory[4096];

    /* Stack for subroutines. */
    Stack stack;

    /* Display memory.
     * Organized as 32 rows of 64 pixels.
     * Each row is 8 bytes.
     */
    uint8_t display[DISPLAY_HEIGHT][DISPLAY_WIDTH / 8];

    /* Flag to indicate if the display has changed.
     */
    bool display_changed;

    /* Should the next instruction be skipped?
     */
    bool skip_next;
} State;

#endif /* _STATE_H */
