#include "game.h"
#include<time.h>
#include<math.h>

static void serveBall(game_t* game){

    (*game).ballX = ((*game).width / 2) - ((*game).ballWidth / 2);
    (*game).ballY = ((*game).height / 2) - ((*game).ballWidth / 2);

    int direction = (rand() % 2) ? 1 : -1;
    float angle = ((rand() % 100) / 100.0 - 0.5);

    double theta = angle * (M_PI / 2);
    game->ballVelX = direction * game->ballSpeed * cos(theta);
    game->ballVelY = game->ballSpeed * sin(theta);
}

void gameInit(game_t* game){
    srand(time(NULL));

    (*game).width = 1080;
    (*game).height = 720;

    (*game).paddleLength = 100;
    (*game).paddleWidth = 20;
    (*game).paddleSpeed = 175;

    (*game).enemyX = 1000.0 - game->paddleWidth;
    (*game).enemyY = (game->height / 2) - (game->paddleLength / 2);

    (*game).playerX = 80;
    (*game).playerY = (game->height / 2) - (game->paddleLength / 2);

    (*game).ballWidth = 20;
    (*game).ballSpeed = 425;

    (*game).playerScore = 0;
    (*game).enemyScore = 0;

    serveBall(game);
}

static void resetGame(game_t* game){
    game->playerScore = 0;
    game->enemyScore = 0;

    game->playerY = (game->height / 2) - (game->paddleLength / 2);
    game->enemyY = (game->height / 2) - (game->paddleLength / 2);

    serveBall(game);
}

static bounds_t playerBoundsCheck(game_t* game){
    if(game->playerY + game->paddleLength >= game->height){
        return BOUNDS_DOWN;
    }else if(game->playerY <= 0){
        return BOUNDS_UP;
    }else{
        return BOUNDS_NONE;
    }
}

static bounds_t enemyBoundsCheck(const game_t* game){
    if(game->enemyY + game->paddleLength >= game->height){
        return BOUNDS_DOWN;
    }else if(game->enemyY <= 0){
        return BOUNDS_UP;
    }else{
        return BOUNDS_NONE;
    }
}

static int ballPaddleCollision(const game_t* game, float px, float py){
    return (game->ballX <= px + game->paddleWidth) 
    && (game->ballX + game->ballWidth >= px)
    && (game->ballY + game->ballWidth >= py)
    && (game->ballY <= py + game->paddleLength);
}

static void bounceOffPaddle(game_t* game, float paddleY, int direction){
    //angle of the bounce depends on where the ball hits the paddle:
    //centre -> flat, top/bottom edge -> steep. relative is in [-1, 1].
    float paddleCentre = paddleY + (game->paddleLength / 2.0f);
    float ballCentre = game->ballY + (game->ballWidth / 2.0f);
    float relative = (ballCentre - paddleCentre) / (game->paddleLength / 2.0f);

    if(relative > 1.0f) relative = 1.0f;
    if(relative < -1.0f) relative = -1.0f;

    double maxBounceAngle = M_PI / 3; //60 degrees
    double bounceAngle = relative * maxBounceAngle;

    //direction: 1 sends the ball right (off player), -1 sends it left (off enemy).
    game->ballVelX = direction * game->ballSpeed * cos(bounceAngle);
    game->ballVelY = game->ballSpeed * sin(bounceAngle);
}

static bounds_t ballBoundsCheck(game_t* game){
    if(game->ballY + game->ballWidth >= game->height){
        return BOUNDS_DOWN;
    }else if(game->ballY <= 0){
        return BOUNDS_UP;
    }else if((game->ballX + game->ballWidth) > game->width){
        return BOUNDS_RIGHT;
    }else if(game->ballX < 0){
        return BOUNDS_LEFT;
    }else if(ballPaddleCollision(game, game->playerX, game->playerY)){
        return PADDLE_LEFT;
    }else if(ballPaddleCollision(game, game->enemyX - game->paddleWidth, game->enemyY)){
        return PADDLE_RIGHT;
    }else{
        return BOUNDS_NONE;
    }
}

static move_t getUpOrDown(){
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_W]) return MOVE_UP;
    if(keys[SDL_SCANCODE_S]) return MOVE_DOWN;
    return MOVE_NONE;
}

static void moveEnemyTowardsBall(game_t* game, double delta){
    bounds_t enemyBounds = enemyBoundsCheck(game);
    if(enemyBounds != BOUNDS_DOWN){
        if(game->ballY > (game->enemyY + (game->paddleLength / 2) + 10)) game->enemyY += game->paddleSpeed * delta;
    }
    if(enemyBounds != BOUNDS_UP){
        if(game->ballY < (game->enemyY + (game->paddleLength / 2) - 10)) game->enemyY -= game->paddleSpeed * delta;
    }
     
}

void update(game_t* game, double delta){
    //player movement
    move_t action = getUpOrDown();
    if(action == MOVE_UP){
        if((playerBoundsCheck(game)) != BOUNDS_UP) game->playerY -= game->paddleSpeed * delta;
    }else if(action == MOVE_DOWN){
        if((playerBoundsCheck(game)) != BOUNDS_DOWN) game->playerY += game->paddleSpeed * delta;
    }

    moveEnemyTowardsBall(game, delta);

    //ball movement
    game->ballX += game->ballVelX * delta;
    game->ballY += game->ballVelY * delta;

    switch(ballBoundsCheck(game)){
        case BOUNDS_UP:
            game->ballY = 0;
            if(game->ballVelY < 0) game->ballVelY = -game->ballVelY;
            break;
        case BOUNDS_DOWN:
            game->ballY = game->height - game->ballWidth;
            if(game->ballVelY > 0) game->ballVelY = -game->ballVelY;
            break;
        case BOUNDS_LEFT:
            game->enemyScore++;
            if(game->enemyScore >= 9) resetGame(game);
            else serveBall(game);
            break;
        case BOUNDS_RIGHT:
            game->playerScore++;
            if(game->playerScore >= 9) resetGame(game);
            else serveBall(game);
            break;
        case PADDLE_LEFT:
            bounceOffPaddle(game, game->playerY, 1);
            break;
        case PADDLE_RIGHT:
            bounceOffPaddle(game, game->enemyY, -1);
            break;
        case BOUNDS_NONE:
            break;
    }
}

static void drawCentreLine(const game_t* game, SDL_Surface* surface){
    int segmentHeight = 20;
    int gap = 15;
    int lineWidth = 4;
    int x = (game->width / 2) - (lineWidth / 2);

    for(int y = 0; y < game->height; y += segmentHeight + gap){
        SDL_Rect segment = {x, y, lineWidth, segmentHeight};
        SDL_FillRect(surface, &segment, 0xFFFFFFFF);
    }
}

static void drawDigit(SDL_Surface* surface, int x, int y, int digit){
    //to draw the numbers, there are no draw text sdl functions.
    //so we will use a 7 segment display for them.
    //we will turn them on using bitwise operations (a first for me!)

    // 0111 1111
    // /gfe dcba
    /*
        a
       b c
        d
       e f
        g
    */

    //0 gfecba
    //1 fc
    //2 gedca
    //3 gfdca
    //4 gfdcb
    //5 gfdba
    //6 gfedba
    //7 fca
    //8 gfedbca
    //9 gfdcba

    static const Uint8 numbers[10] = {
        0x77,   // 0
        0x24,   // 1
        0x5D,   // 2
        0x6D,   // 3
        0x2E,   // 4
        0x6B,   // 5
        0x7B,   // 6
        0x25,   // 7
        0x7F,   // 8
        0x6F    // 9
    };

    int segmentT = 6;
    int gap = 1;
    int segmentHeight = segmentT * 5;

    const int xLeft   = x;
    const int xMid    = x + segmentT + gap;
    const int xRight  = x + segmentT + (gap * 2) + segmentHeight;

    const int yTop    = y;
    const int yUpper  = y + segmentT + gap;
    const int yMid    = y + segmentT + (gap * 2) + segmentHeight;
    const int yLower  = y + (segmentT * 2) + (gap * 3) + segmentHeight;
    const int yBottom = y + (segmentT * 2) + (gap * 4) + (segmentHeight * 2);

    const Uint8 mask = numbers[digit];

    // horizontal segments
    if (mask & (1u << 0)) SDL_FillRect(surface, &(SDL_Rect){xMid, yTop, segmentHeight, segmentT}, 0xFFFFFFFF);
    if (mask & (1u << 3)) SDL_FillRect(surface, &(SDL_Rect){xMid, yMid, segmentHeight, segmentT}, 0xFFFFFFFF);
    if (mask & (1u << 6)) SDL_FillRect(surface, &(SDL_Rect){xMid, yBottom, segmentHeight, segmentT}, 0xFFFFFFFF);

    // left segments
    if (mask & (1u << 1)) SDL_FillRect(surface, &(SDL_Rect){xLeft, yUpper, segmentT, segmentHeight}, 0xFFFFFFFF);
    if (mask & (1u << 4)) SDL_FillRect(surface, &(SDL_Rect){xLeft, yLower, segmentT, segmentHeight}, 0xFFFFFFFF);

    // right segments
    if (mask & (1u << 2)) SDL_FillRect(surface, &(SDL_Rect){xRight, yUpper, segmentT, segmentHeight}, 0xFFFFFFFF);
    if (mask & (1u << 5)) SDL_FillRect(surface, &(SDL_Rect){xRight, yLower, segmentT, segmentHeight}, 0xFFFFFFFF);
}

static void drawScores(const game_t* game, SDL_Surface* surface){
    int segmentT = 6;
    int segmentHeight = segmentT * 5;
    int totalWidth = (segmentT * 2) + segmentHeight;
    int scoreY = 20;

    int playerScoreX = game->width / 4 - totalWidth / 2;
    int enemyScoreX  = (game->width * 3) / 4 - totalWidth / 2;

    drawDigit(surface, playerScoreX, scoreY, game->playerScore);
    drawDigit(surface, enemyScoreX, scoreY, game->enemyScore);
}

void render(game_t* game, SDL_Window* window, SDL_Surface* surface){
    SDL_FillRect(surface, NULL, 0x00000000);//clear screen.

    drawCentreLine(game, surface);
    drawScores(game, surface);

    SDL_Rect player = {(int)game->playerX,(int)game->playerY,game->paddleWidth,game->paddleLength};
    SDL_Rect enemy = {(int)game->enemyX - (int)game->paddleWidth,(int)game->enemyY,game->paddleWidth,game->paddleLength};
    SDL_Rect ball = {(int)game->ballX, game->ballY, game->ballWidth, game->ballWidth};

    SDL_FillRect(surface, &player, 0xFFFFFFFF);
    SDL_FillRect(surface, &enemy, 0xFFFFFFFF);
    SDL_FillRect(surface, &ball, 0xFFFFFFFF);
    SDL_UpdateWindowSurface(window);
}
