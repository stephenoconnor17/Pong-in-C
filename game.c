#include "game.h"
#include<time.h>
#include<math.h>

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

    (*game).playerScore = 0;
    (*game).enemyScore = 0;

    serveBall(game);
}

void serveBall(game_t* game){
    (*game).ballX = ((*game).width / 2) - ((*game).ballWidth / 2);
    (*game).ballY = ((*game).height / 2) - ((*game).ballWidth / 2);

    int direction = (rand() % 2) ? 1 : -1;
    float angle = ((rand() % 100) / 100.0 - 0.5);

    double theta = angle * (M_PI / 2);
    game->ballVelX = direction * 200 * cos(theta);
    game->ballVelY = 200 * sin(theta);
}

bounds_t playerBoundsCheck(game_t* game){
    if(game->playerY + game->paddleLength >= game->height){
        return BOUNDS_DOWN;
    }else if(game->playerY <= 0){
        return BOUNDS_UP;
    }else{
        return BOUNDS_NONE;
    }
}

bounds_t enemyBoundsCheck(game_t* game){
    if(game->enemyY + game->paddleLength >= game->height){
        return BOUNDS_DOWN;
    }else if(game->enemyY <= 0){
        return BOUNDS_UP;
    }else{
        return BOUNDS_NONE;
    }
}

bounds_t ballBoundsCheck(game_t* game){
    if(game->ballY + game->ballWidth >= game->height){
        return BOUNDS_DOWN;
    }else if(game->ballY <= 0){
        return BOUNDS_UP;
    }else if((game->ballX + game->ballWidth) > game->width){
        return BOUNDS_RIGHT;
    }else if(game->ballX < 0){
        return BOUNDS_LEFT;
    }else{
        return BOUNDS_NONE;
    }
}

move_t getUpOrDown(){
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_W]) return MOVE_UP;
    if(keys[SDL_SCANCODE_S]) return MOVE_DOWN;
    return MOVE_NONE;
}

void update(game_t* game, double delta){
    //player movement
    move_t action = getUpOrDown();
    if(action == MOVE_UP){
        if((playerBoundsCheck(game)) != BOUNDS_UP) game->playerY -= 250 * delta;
    }else if(action == MOVE_DOWN){
        if((playerBoundsCheck(game)) != BOUNDS_DOWN) game->playerY += 250 * delta;
    }

    //ball movement
    game->ballX += game->ballVelX * delta;
    game->ballY += game->ballVelY * delta;

    switch(ballBoundsCheck(game)){
        case BOUNDS_UP: case BOUNDS_DOWN:
            game->ballVelY = -game->ballVelY;
            break;
        case BOUNDS_LEFT:
            game->enemyScore++;
            serveBall(game);
            break;
        case BOUNDS_RIGHT:
            game->playerScore++;
            serveBall(game);
            break;
        case BOUNDS_NONE:
            break;
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
