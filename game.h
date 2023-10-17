#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display.h"

typedef enum 
{
   DEAD,
   ALIVE
} GameState;

typedef struct GameStruct
{
   int width;
   int height;
   GameState state;

   int **lastgen;
   int **currgen;
   int **nextgen;
} Game;

GameState checkGameState(Game *game);
int checkNeighbor(int bw, int bh, int x, int y, int **cells);
void copyBoard(int **dest, int **src, int width, int height);
int countNeighbors(int bw, int bh, int x, int y, int **cells);
int **destroyBoard(int **board, int width);
Game *destroyGame(Game *game);
int **initBoard(int width, int height);
void populateBoard(int **board, int width, int height);
int runGame(SDL_Renderer *ren);
Game *setupGame();
void updateGame(Game *game);

#endif