/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014 Ramon Santamaria (Ray San - raysan@raysanweb.com)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include "../libs/1dgameplay.h"
#include <stdio.h>

//#define WALLS_POOL_LENGHT 3
#define WALLS_MIN_LIMIT 200
#define WALLS_MAX_LIMIT 300
#define WALLS_LIMIT_LENGHT 2
#define WALLS_COLOR RED
#define WALLS_LIMIT_COLOR GREEN

#define PLAYER_GAME_LENGHT 7
#define PLAYER_JUMP_COUNTER 25
#define PLAYER_JUMP_SPEED 30

#define JUMP_KEY KEY_SPACE
#define GRAVITY_SPEED 2

#define EFFECT_DURATION 120

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------

// Gameplay screen global variables
static int framesCounter;
static int finishScreen;

// Gameplay variables
int midScreen;
int groundPosition;
float gameSpeed;
int wallsSpawnCounter;
int wallsSpawnCounterMax;

Effect effect;

Player player;
//Wall wallsPool[WALLS_POOL_LENGHT];
Wall wall;
ScoreUI score;

enum GameStates gameState;
int gs;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
    
    gameSpeed = 1;
    wallsSpawnCounterMax = 60 * 1;
    wallsSpawnCounter = wallsSpawnCounterMax;
    
    gameState = WAITING;
    
    InitEffect(&effect, EFFECT_DURATION, 10);
    InitScore(&score, 0, 4, 2, GREEN, BLACK);
    
    midScreen = GetScreenWidth() / 2;
    groundPosition = GetScreenHeight();
    InitPlayer(&player, PLAYER_GAME_LENGHT, PLAYER_JUMP_COUNTER, PLAYER_JUMP_SPEED, GRAVITY_SPEED, BLUE);
    InitWall(&wall, WALLS_MIN_LIMIT, WALLS_MAX_LIMIT, groundPosition, WALLS_LIMIT_LENGHT, WALLS_COLOR, WALLS_LIMIT_COLOR);
    //InitWalls(wallsPool, WALLS_POOL_LENGHT, WALLS_MIN_LENGHT, WALLS_MAX_LENGHT, groundPosition, WALLS_LIMIT_LENGHT, WALLS_COLOR, WALLS_LIMIT_COLOR);
}

void ResetGameplayScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
    
    gameSpeed = 1;
    wallsSpawnCounterMax = 60 * 1;
    wallsSpawnCounter = wallsSpawnCounterMax;
    
    gameState = WAITING;
    
    InitEffect(&effect, EFFECT_DURATION, 10);
    InitScore(&score, 0, 4, 2, GREEN, BLACK);
    
    midScreen = GetScreenWidth() / 2;
    groundPosition = GetScreenHeight();
    InitPlayer(&player, PLAYER_GAME_LENGHT, PLAYER_JUMP_COUNTER, PLAYER_JUMP_SPEED, GRAVITY_SPEED, BLUE);
    InitWall(&wall, WALLS_MIN_LIMIT, WALLS_MAX_LIMIT, groundPosition, WALLS_LIMIT_LENGHT, WALLS_COLOR, WALLS_LIMIT_COLOR);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{   
    //TODO: TOP AND BOT WALL (FLappyBIrd-Like)
        
    switch(gameState)
    {
        case WAITING: 
        
            if (IsKeyPressed(JUMP_KEY)) gameState = ALIVE;
        
        break;
        
        case ALIVE: 
        
            if (wallsSpawnCounter >= wallsSpawnCounterMax)
            {
                // Spawn a new wall
                ResetWall(&wall, WALLS_MIN_LIMIT, WALLS_MAX_LIMIT, gameSpeed);
                wallsSpawnCounter = 0;
            }
            else if (!wall.isActive)
            {
                wallsSpawnCounter += gameSpeed;
            }
            
            if (UpdateWall(&wall, gameSpeed))
            {
                IncreaseScore(&score, &gameSpeed, &wallsSpawnCounterMax, &gameState);
                //gameState = LEVEL_UP;
            }
            //UpdateWalls(wallsPool, WALLS_POOL_LENGHT, gameSpeed);
            
            PlayerJump(&player, JUMP_KEY);
            
            AddGravity(&player, GRAVITY_SPEED);
            SetPosition(&player);
            
            
            if (PlayerGroundCollisionDetection(player, GetScreenHeight()))
            {
                PlayerGroundCollisionResolution(&player, groundPosition);
            }
            
            if (PlayerWallCollisionDetection(player, wall)) gameState = DEFEAT;
            if (gameState != ALIVE) SetEffect(&effect, GetPlayerMidPosition(player), GetScreenHeight() - GetPlayerMidPosition(player));
                
        
        break;
        
        case LEVEL_UP:
        
        if (UpdateEffect(&effect)) gameState = ALIVE;
        //printf("aaaaaaaaaaaaaaaaaaaaaaaa");
        
        break;
        
        case DEFEAT:
        
        if (UpdateEffect(&effect)) ResetGameplayScreen();
        //if (UpdateEffect(&effect)) gameState = ALIVE;
        
        break;
        
        case VICTORY:
        
        UpdateEffect(&effect);
        
        break;
        
        default: break;
    }
    
    // Press enter to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER))
    {
        finishScreen = 1;
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    DrawScore(score, midScreen, GAME_WIDTH);
    
    switch(gameState)
    {
        case WAITING: 
        
        break;
        
        case ALIVE: 
       
        DrawWall(&wall, midScreen, GAME_WIDTH);
        //DrawWalls(wallsPool, WALLS_POOL_LENGHT, midScreen, GAME_WIDTH);
        DrawRectangle1D(player.position - player.lenght/2, player.lenght, BLUE, midScreen, GAME_WIDTH, true);
        
        break;
        
        case LEVEL_UP:
        
        DrawWall(&wall, midScreen, GAME_WIDTH);
        //DrawWalls(wallsPool, WALLS_POOL_LENGHT, midScreen, GAME_WIDTH);
        DrawRectangle1D(player.position - player.lenght/2, player.lenght, BLUE, midScreen, GAME_WIDTH, true);
        
        DrawEffect(effect, GetPlayerMidPosition(player), midScreen, GAME_WIDTH, GREEN);
        
        break;
        
        case DEFEAT:
        
        DrawWall(&wall, midScreen, GAME_WIDTH);
        //DrawWalls(wallsPool, WALLS_POOL_LENGHT, midScreen, GAME_WIDTH);
        DrawRectangle1D(player.position - player.lenght/2, player.lenght, BLUE, midScreen, GAME_WIDTH, true);
        DrawEffect(effect, GetPlayerMidPosition(player), midScreen, GAME_WIDTH, RED);
        
        break;
        
        case VICTORY:
        
        DrawEffect(effect, GetPlayerMidPosition(player), midScreen, GAME_WIDTH, YELLOW);
        
        break;
        
        default: break;
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{

}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}