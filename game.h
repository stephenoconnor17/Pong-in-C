#ifndef game_h
#define game_h
#include <SDL2/SDL.h>

typedef struct game{
    int width;
    int height;

    int playerX;
    int playerY;

    int enemyX;
    int enemyY;

    int paddleWidth;
    int paddleLength;

    int ballWidth;
    int ballX;
    int ballY;

    int playerScore;
    int enemyScore;
}game_t;

void gameInit(game_t* game);
void update(game_t* game, double delta);
int getUpOrDown();
int calculateTrajectory();
void render(game_t* game, SDL_Window* window, SDL_Surface* surface);

#endif