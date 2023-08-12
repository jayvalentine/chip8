#include "include/state.h"

#include <stdio.h>
#include <unistd.h>

void platform_init()
{

}

bool platform_tick()
{
    return false;
}

void platform_quit()
{

}

void platform_update_display(State * s)
{
    printf("\033[2J\033[1;1H");

    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < DISPLAY_WIDTH / 8; x++)
        {
            char b = s->display[y][x];
            for (int i = 0; i < 8; i++)
            {
                if (0b10000000 & b) printf("\033[1;47m ");
                else printf("\033[1;40m ");

                b <<= 1;
            }
        }
        putchar('\n');
    }
}

void platform_delay(uint32_t delay_ms)
{
    usleep(delay_ms * 1000);
}
