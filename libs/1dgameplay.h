// 1d functions on a 2d space
//#ifndef 1DGAMEPLAY_H
//#define 1DGAMEPLAY_H

#include "raylib.h"
#include <stdio.h>
#include "ceasings.h"

#define SCORE_LENGHT 3
#define SUB_SCORE_MAX 1
#define GAME_SPEED_INCREMENT 0.2f
#define SPAWN_COUNTER_DECREMENT 5


// STRUCTS----------------------------------------------------------------------

enum GameStates {WAITING = 0, ALIVE, LEVEL_UP, DEFEAT, VICTORY};

typedef struct
{
    bool isJumping;
    float speed;
    float compSpeed;
    int counter;
    int counterMax;
}Jump;

typedef struct
{
    float position;
    float velocity;
    int lenght;
    Jump jump;
    Color color;
    bool isGrounded;
    bool isAlive;
    bool isActive;
}Player;

typedef struct 
{
    int position;
    int lenght;
    Color color;
}Limit;

typedef struct 
{
    int position;
    float lenght;
    Limit limit;
    Color color;
    float alphaIncrement;
    float alpha;
    bool isActive;
}Wall;

typedef struct 
{
    int position;
    int lenght;
    int value;
    Color color;
}Score;

typedef struct 
{
    int position;
    int lenght;
    int scoreIndex;
    Score score[SCORE_LENGHT];
    Color color;
    bool isActive;
}ScoreUI;

typedef struct 
{
    int t;
    int d;
    int lenghtB;
    int lenghtC;
    int colorB;
    int colorC;
    int lenght;
    Color color;
}Effect;

//------------------------------------------------------------------------------


// FUNCTIONS--------------------------------------------------------------------

// Move the position "speed" times
void Translate(float *position, float speed);
void DrawRectangle1D(int position, int lenght, Color color, int midScreen, int gameWidth, bool veticalOrientation);
void InitPlayer(Player *p, int lenght, int jumpCounterMax, float jumpSpeed, float gravity, Color color);
void PlayerJump(Player *p, int jumpKey);
void AddGravity(Player *p, float gravity);
void SetPosition(Player *p);
bool PlayerGroundCollisionDetection(Player p, int ground);
void PlayerGroundCollisionResolution(Player *p, int ground);
void InitWalls(Wall walls[], int wallsDim,int minLenght, int maxLenght, int ground, int limitLenght, Color wallColor, Color limitColor);
void InitWall(Wall *w, int minLenght, int maxLenght, int ground, int limitLenght, Color wallColor, Color limitColor);
void UpdateWalls(Wall walls[], int wallsDim, float gameSpeed);
bool UpdateWall(Wall *w, float gameSpeed);
void SpawnWall(Wall walls[], int wallsDim, int minLenght, int maxLenght, float gameSpeed);
void ResetWall(Wall *w, int minLenght, int maxLenght, float gameSpeed);
void DrawWalls(Wall walls[], int wallsDim, int midScreen, int gameWidth);
void DrawWall(Wall *w, int midScreen, int gameWidth);
bool PlayerWallCollisionDetection(Player p, Wall w);
void InitScore(ScoreUI *s, int position, int subLenght, int offset, Color color, Color bgColor);
void IncreaseScore(ScoreUI *s, float *gameSpeed, int *spawnCounter, int *gameState);
void DrawScore(ScoreUI s, int midScreen, int gameWidth);
void DrawEffect(Effect e, int p, int midScreen, int gameWidth);
void InitEffect(Effect *e, int d, int lenghtC, int colorB, Color color);
bool UpdateEffect(Effect *e);

//------------------------------------------------------------------------------


//#endif