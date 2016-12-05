// 1d functions

#include "1dgameplay.h"

void Translate(float *position, float speed)
{
    *position += speed;
}

void DrawRectangle1D(int position, int lenght, Color color, int midScreen, int gameWidth, bool verticalOrientation)
{   
    if (verticalOrientation) DrawRectangle(midScreen-gameWidth/2, position, gameWidth, lenght, color);
    else DrawRectangle(midScreen-gameWidth/2, position, lenght, gameWidth, color);
}

void InitPlayer(Player *p, int lenght, int jumpCounterMax, float jumpSpeed, float gravity, Color color)
{
    (*p).velocity = 0;
    (*p).lenght = lenght;
    (*p).position = GetScreenHeight() - lenght/2;
    (*p).jump.isJumping = false;
    (*p).jump.speed = jumpSpeed;
    (*p).jump.compSpeed = gravity/2;
    (*p).jump.counter = 0;
    (*p).jump.counterMax = jumpCounterMax;
    (*p).color = color;
    (*p).isGrounded = false;
    (*p).isAlive = true;
    (*p).isActive = true;
}

void PlayerJump(Player *p, int jumpKey)
{
    if ((*p).isGrounded && IsKeyPressed(jumpKey))
    {
        (*p).velocity = -(*p).jump.speed;
        (*p).jump.counter = (*p).jump.counterMax;
        (*p).jump.isJumping = true;
        (*p).isGrounded = false;
    }
    else if ((*p).jump.isJumping && (*p).jump.counter > 0 && IsKeyDown(jumpKey))
    {
        (*p).velocity -= (*p).jump.compSpeed;
        (*p).jump.counter--;
    }
    else if ((*p).jump.isJumping)
    {
        (*p).jump.counter = 0;
        (*p).jump.isJumping = false;
    }
}

void AddGravity(Player *p, float gravity)
{
    if (!(*p).isGrounded)
    {
        (*p).velocity += gravity;
    }
}

void SetPosition(Player *p)
{
    (*p).position += (*p).velocity;
}

bool PlayerGroundCollisionDetection(Player p, int ground)
{
    if (!p.isGrounded && p.position + p.lenght/2 >= ground) return true;
    return false;
}

void PlayerGroundCollisionResolution(Player *p, int ground)
{
    (*p).position = ground - (*p).lenght/2;
    (*p).isGrounded = true;
    (*p).velocity = 0;
}

void InitWalls(Wall walls[], int wallsDim,int minLenght, int maxLenght, int ground, int limitLenght, Color wallColor, Color limitColor)
{
    for (int i=0; i<wallsDim; i++)
    {
        walls[i].position = ground;
        walls[i].lenght = 0;
        walls[i].limit.position = GetRandomValue(minLenght, maxLenght);
        walls[i].limit.lenght = limitLenght;
        walls[i].limit.color = limitColor;
        walls[i].limit.color.a = 0;
        walls[i].alphaIncrement = 0;
        walls[i].alpha = 0;
        walls[i].color = wallColor;
        walls[i].color.a = 0;
        walls[i].isActive = false;
    }
}

void InitWall(Wall *w, int minLenght, int maxLenght, int ground, int limitLenght, Color wallColor, Color limitColor)
{
    (*w).position = ground;
    (*w).lenght = 0;
    (*w).limit.position = GetRandomValue(minLenght, maxLenght);
    (*w).limit.lenght = limitLenght;
    (*w).limit.color = limitColor;
    (*w).limit.color.a = 0;
    (*w).alphaIncrement = 0;
    (*w).alpha = 0;
    (*w).color = wallColor;
    (*w).color.a = 0;
    (*w).isActive = false;
}

void UpdateWalls(Wall walls[], int wallsDim, float gameSpeed)
{
    for (int i=0; i<wallsDim; i++)
    {
        if (walls[i].isActive)
        {
            if (walls[i].position-walls[i].lenght > walls[i].limit.position)
            {
                walls[i].lenght += gameSpeed;
                walls[i].alpha += walls[i].alphaIncrement;
                walls[i].color.a = walls[i].alpha;
                walls[i].limit.color.a = walls[i].alpha;
            }
            else walls[i].isActive = false;
        }
    }
}

bool UpdateWall(Wall *w, float gameSpeed)
{
    if ((*w).isActive)
    {
        if ((*w).position-(*w).lenght > (*w).limit.position)
        {
            (*w).lenght += gameSpeed;                                            
            (*w).alpha += (*w).alphaIncrement;
            (*w).color.a = (*w).alpha;
            (*w).limit.color.a = (*w).alpha;
        }
        else 
        {
            (*w).isActive = false;
            return true;
        }
    }
    return false;
}

void SpawnWall(Wall walls[], int wallsDim, int minLenght, int maxLenght, float gameSpeed)
{
    int i = 0;
    while(walls[i].isActive && i<wallsDim)
    {
        i++;
    }
    
    if (i!=wallsDim) ResetWall(&walls[i], minLenght, maxLenght, gameSpeed);
}

void ResetWall(Wall *w, int minLenght, int maxLenght, float gameSpeed)
{
    (*w).lenght = 0;
    (*w).limit.position = GetRandomValue(minLenght, maxLenght);
    (*w).color.a = 0;
    (*w).limit.color.a = 0;
    (*w).alphaIncrement = 255 / (float)(((*w).position + (*w).lenght -(*w).limit.position)/gameSpeed);
    (*w).alpha = 0;
    (*w).isActive = true;
}

void DrawWalls(Wall walls[], int wallsDim, int midScreen, int gameWidth)
{
    for (int i=0; i<wallsDim; i++)
    {
        if (walls[i].isActive) 
        {
            DrawRectangle1D(walls[i].position - walls[i].lenght, walls[i].lenght, walls[i].color, midScreen, gameWidth, true);
            DrawRectangle1D(walls[i].limit.position - walls[i].limit.lenght, walls[i].limit.lenght, walls[i].limit.color, midScreen, gameWidth, true);
        }
    }
}

void DrawWall(Wall *w, int midScreen, int gameWidth)
{
    if ((*w).isActive)
    {
        DrawRectangle1D((*w).position - (*w).lenght, (*w).lenght, (*w).color, midScreen, gameWidth, true);
        DrawRectangle1D((*w).limit.position - (*w).limit.lenght, (*w).limit.lenght, (*w).limit.color, midScreen, gameWidth, true);
    }
}

bool PlayerWallCollisionDetection(Player p, Wall w)
{
    if (w.isActive && w.position - w.lenght <= w.limit.position)
    {
        if (p.position > w.limit.position)
        {
            // Player has collided with the wall
            
            return true;
        }
    }
    return false;
}

void InitScore(ScoreUI *s, int position, int subLenght, int offset, Color color, Color bgColor)
{
    (*s).position = position;
    (*s).lenght = subLenght * SCORE_LENGHT + offset * (SCORE_LENGHT + 1);
    (*s).scoreIndex = 0;
    for (int i=0; i<SCORE_LENGHT; i++)
    {
        (*s).score[i].position = i*subLenght + (i + 1) * offset;
        (*s).score[i].lenght = subLenght;
        (*s).score[i].value = 0;
        (*s).score[i].color = color;
        (*s).score[i].color.a = 0;
    }
    (*s).color = bgColor;
    (*s).isActive = true;
}

void IncreaseScore(ScoreUI *s, float *gameSpeed, int *spawnCounter, int *gameState)
{
    (*s).score[(*s).scoreIndex].value++;
    (*s).score[(*s).scoreIndex].color.a = (*s).score[(*s).scoreIndex].value * 255/SUB_SCORE_MAX;
    *gameSpeed += (float)GAME_SPEED_INCREMENT;
    
    if ((*s).score[(*s).scoreIndex].value >= SUB_SCORE_MAX)
    {
        (*s).scoreIndex++;
        if((*s).scoreIndex >= SCORE_LENGHT)
        {
            // WIN
            gameState = (int)VICTORY;
        }
        else
        {
            *spawnCounter -= SPAWN_COUNTER_DECREMENT;
            *gameState = LEVEL_UP;
        }
    }
}

void DrawScore(ScoreUI s, int midScreen, int gameWidth)
{
    DrawRectangle1D(s.position, s.lenght, s.color, midScreen, gameWidth, true);
    
    for (int i=0; i<SCORE_LENGHT; i++)
    {
        DrawRectangle1D(s.score[i].position, s.score[i].lenght,s.score[i].color , midScreen, gameWidth, true);
    }
}

void InitEffect(Effect *e, int d, int alphaB)
{
    (*e).t = 0;
    (*e).d = d;
    (*e).lenghtB = 0;
    (*e).lenghtUc = 0;
    (*e).lenghtLc = 0;
    (*e).alphaB = 255;//alphaB;
    (*e).alphaC = 255 - alphaB;
    (*e).lenghtU = 0;
    (*e).lenghtL = 0;
    (*e).alpha = 0;
}

void SetEffect(Effect *e, int lenghtU, int lenghtL)
{
    (*e).t = 0;
    (*e).lenghtUc = lenghtU;
    (*e).lenghtLc = lenghtL;
    (*e).lenghtU = 0;
    (*e).lenghtL = 0;
    (*e).alpha = 0;
}

bool UpdateEffect(Effect *e)
{
    if ((*e).t <= (*e).d)
    {
        (*e).lenghtU = (int)BounceEaseInOut((*e).t, (*e).lenghtB, (*e).lenghtUc, (*e).d);
        (*e).lenghtL = (int)BounceEaseInOut((*e).t, (*e).lenghtB, (*e).lenghtLc, (*e).d);
        (*e).alpha = (int)QuadEaseIn((*e).t, (*e).alphaB, (*e).alphaC, (*e).d);
        (*e).t++;
        
        return false;
    }
    
    return true;
}

void DrawEffect(Effect e, int p, int midScreen, int gameWidth, Color color)
{
    color.a = e.alpha;
    DrawRectangle1D(p - e.lenghtU, e.lenghtU, color, midScreen, gameWidth, true);
    DrawRectangle1D(p, e.lenghtL, color, midScreen, gameWidth, true);
}

int GetPlayerMidPosition(Player p)
{
    return p.position + p.lenght/2;
}