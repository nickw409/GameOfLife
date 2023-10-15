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

GameState checkDeadGame(int **lastgen, int **nextgen, int width, int height);
int checkNeighbor(int bw, int bh, int x, int y, int **cells);
int countNeighbors(int bw, int bh, int x, int y, int **cells);
void destroyBoard(int **board, int width);
int **initBoard(int width, int height);
int runGame(SDL_Renderer *ren);
void setupBoard(int **board, int width, int height);
void updateBoard(int bw, int bh, int **lastgen, int **currgen, int **nextgen);

#endif