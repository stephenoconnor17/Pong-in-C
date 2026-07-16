#include "game.h"
#include<time.h>

void gameInit(game_t* game){
    srand(time(NULL));

    (*game).width = 1080;
    (*game).height = 720;

    (*game).paddleLength = 100;
    (*game).paddleWidth = 20;

    (*game).enemyX = 1000.0 - game->paddleWidth;
    (*game).enemyY = (game->height / 2) - (game->paddleLength / 2);

    (*game).playerX = 80;
    (*game).playerY = (game->height / 2) - (game->paddleLength / 2);

    (*game).ballWidth = 10;
    (*game).ballX = ((*game).width / 2) - ((*game).ballWidth / 2);
    (*game).ballY = ((*game).height / 2) - ((*game).ballWidth / 2);

    (*game).playerScore = 0;
    (*game).enemyScore = 0;
}

int calculateTrajectory(){

}

int playerBoundsCheck(game_t* game){
    if(game->playerY + game->paddleLength >= game->height){
        return 1;
    }else if(game->playerY <= 0){
        return 2;
    }else{
        return 0;
    }
}

int getUpOrDown(){
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_W]) return 2;
    if(keys[SDL_SCANCODE_S]) return 1;
    return 0;
}

void update(game_t* game, double delta){
    int action = getUpOrDown();
    if(action == 2){
        if((playerBoundsCheck(game)) != 2) game->playerY -= 250 * delta;
    }else if(action == 1){
        if((playerBoundsCheck(game)) != 1) game->playerY += 250 * delta;
    }
}

void render(game_t* game, SDL_Window* window, SDL_Surface* surface){
    SDL_FillRect(surface, NULL, 0x00000000);//clear screen.

    SDL_Rect player = {(int)game->playerX,(int)game->playerY,game->paddleWidth,game->paddleLength};
    SDL_Rect enemy = {(int)game->enemyX - (int)game->paddleWidth,(int)game->enemyY,game->paddleWidth,game->paddleLength};
    SDL_Rect ball = {(int)game->ballX, game->ballY, game->ballWidth, game->ballWidth};

    SDL_FillRect(surface, &player, 0xFFFFFFFF);
    SDL_FillRect(surface, &enemy, 0xFFFFFFFF);
    SDL_FillRect(surface, &ball, 0xFFFFFFFF);
    SDL_UpdateWindowSurface(window);
}
