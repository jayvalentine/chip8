#ifndef _STATE_H
#define _STATE_H

#include "stdbool.h"
#include "stdint.h"

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE (DISPLAY_HEIGHT * (DISPLAY_WIDTH / 8))

typedef uint16_t address;

/* Represents the emulator state. */
typedef struct _State
{
    address pc;

    /* Registers 0 through F. */
    uint8_t registers[16];

    /* Display memory.
     * Organized as 32 rows of 64 pixels.
     * Each row is 8 bytes.
     */
    uint8_t display[DISPLAY_HEIGHT][DISPLAY_WIDTH / 8];

    /* Flag to indicate if the display has changed.
     */
    bool display_changed;
} State;

#endif /* _STATE_H */
