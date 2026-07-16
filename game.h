#ifndef game_h
#define game_h
#include <SDL2/SDL.h>

typedef struct game{
    int width;
    int height;

    float playerX;
    float playerY;

    float enemyX;
    float enemyY;

    int paddleWidth;
    int paddleLength;

    int ballWidth;
    float ballX;
    float ballY;

    int playerScore;
    int enemyScore;
}game_t;

void gameInit(game_t* game);
void update(game_t* game, double delta);
int getUpOrDown();
int calculateTrajectory();
void render(game_t* game, SDL_Window* window, SDL_Surface* surface);
int playerBoundsCheck(game_t* game);

#endif