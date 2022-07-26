/* Platform-specific functionality for SDL2. */

#include "include/state.h"

#include <SDL2/SDL.h>

/* Width and height of emulator "pixels" in real pixels. */
#define PIXEL_WIDTH 8
#define PIXEL_HEIGHT 8

#define WINDOW_WIDTH (DISPLAY_WIDTH * PIXEL_WIDTH)
#define WINDOW_HEIGHT (DISPLAY_HEIGHT * PIXEL_HEIGHT)

SDL_Renderer *renderer;
SDL_Window *window;

void platform_init()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
}

bool platform_tick()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return true;
        }
    }

    return false;
}

void platform_quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_pixel(int x, int y)
{
    int y_start = y * PIXEL_HEIGHT;
    int y_end = y_start + PIXEL_HEIGHT;
    int x_start = x * PIXEL_WIDTH;
    int x_end = x_start + PIXEL_WIDTH;

    for (int ry = y_start; ry < y_end; ry++)
    {
        for (int rx = x_start; rx < x_end; rx++)
        {
            SDL_RenderDrawPoint(renderer, rx, ry);
        }
    }
}

void platform_update_display(State * s)
{
    /* Clear screen. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    /* Set pixel colour to white. */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    /* Reset to top-right corner. */
    /* Iterate over each row in the display. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        int x = 0;

        /* Iterate over each byte in the row. */
        for (int x_in_bytes = 0; x_in_bytes < DISPLAY_WIDTH / 8; x_in_bytes++)
        {
            uint8_t b = s->display[y][x_in_bytes];
            for (int i = 0; i < 8; i++)
            {
                if (b & 0b10000000)
                {
                    draw_pixel(x, y);
                }

                b <<= 1;
                x++;
            }
        }
    }

    /* Present to screen now that drawing is done. */
    SDL_RenderPresent(renderer);
}
