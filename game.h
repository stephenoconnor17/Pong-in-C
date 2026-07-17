#ifndef game_h
#define game_h
#include <SDL2/SDL.h>

typedef enum{
    BOUNDS_UP,
    BOUNDS_DOWN,
    BOUNDS_LEFT,
    BOUNDS_RIGHT,
    BOUNDS_NONE
}bounds_t;

typedef enum{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_NONE
}move_t;

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
    float ballVelX;
    float ballVelY;

    int playerScore;
    int enemyScore;

    double theta;
}game_t;

void gameInit(game_t* game);
void update(game_t* game, double delta);
move_t getUpOrDown();
void serveBall();
void render(game_t* game, SDL_Window* window, SDL_Surface* surface);
bounds_t playerBoundsCheck(game_t* game);
bounds_t ballBoundsCheck(game_t* game);
bounds_t enemyBoundsCheck(game_t* game);

#endif