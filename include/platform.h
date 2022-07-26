#ifndef _PLATFORM_H
#define _PLATFORM_H

/* Platform-specific functionality.
 * These functions are implemented by each of the platforms
 * to provide a common interface for the CHIP-8 emulator.
 */

/* Performs any platform-specific initialization. */
void platform_init();

/* Performs any platform-specific per-cycle updates. */
bool platform_tick();

/* Performs any platform-specific cleanup. */
void platform_quit();

/* Updates the display according to the current state. */
void platform_update_display(State * s);

#endif /* _PLATFORM_H */
