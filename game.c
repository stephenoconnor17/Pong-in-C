#include "game.h"

void gameInit(game_t* game){
    (*game).width = 1080;
    (*game).height = 720;

    (*game).playerX = 80;
    (*game).playerY = 100;

    (*game).enemyX = 1000;
    (*game).enemyY = 100;

    (*game).paddleLength = 100;
    (*game).paddleWidth = 20;

    (*game).ballWidth = 10;
    (*game).ballX = ((*game).width / 2) - ((*game).ballWidth / 2);
    (*game).ballY = ((*game).height / 2) - ((*game).ballWidth / 2);

    (*game).playerScore = 0;
    (*game).enemyScore = 0;
}

void update(game_t* game, double delta){

}

int getUpOrDown(){

}

int calculateTrajectory(){

}

void render(game_t* game, SDL_Window* window, SDL_Surface* surface){

}
