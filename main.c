#include <SDL2/SDL.h>
#include <stdbool.h>
#include "game.h"

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("SDL Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1080, 720, SDL_WINDOW_SHOWN);

    if (!win) {
        fprintf(stderr, "CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }


    SDL_Surface* surface = SDL_GetWindowSurface(win);

    game_t game;
    gameInit(&game);

    bool running = true;

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 last;
    double delta;

    while (running) {
        last = now;
        now = SDL_GetPerformanceCounter();
        delta = (double)(now - last) / (double)(freq);

        update(&game, delta);
        render(&game, win, surface);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }       
        }
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}