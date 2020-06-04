#include "display.h"
#include <stdio.h>
#include <stdlib.h>

int checkNeighbor(int bw, int bh, int x, int y, int cells[bh][bw])
{
   if (x >= 0 && y >= 0 && x < bw && y < bh && cells[y][x] == 1)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

int countNeighbors(int bw, int bh, int x, int y, int cells[bh][bw])
{
   int count = 0;
   if (checkNeighbor(bw, bh, x-1, y-1, cells) == 1)
   {
      ++count;
   }
   if (checkNeighbor(bw, bh, x, y-1, cells) == 1)
   {
      ++count;
   }
   if (checkNeighbor(bw, bh, x+1, y-1, cells) == 1)
   {
      ++count;
   }
   if (checkNeighbor(bw, bh, x-1, y, cells) == 1)
   {
      ++count;
   }
   if (checkNeighbor(bw, bh, x, y, cells) == 1)
   {
      ++count;
   }
   if (checkNeighbor(bw, bh, x+1, y, cells) == 1)
   {
      ++count;
   }
   if (checkNeighbor(bw, bh, x-1, y+1, cells) == 1)
   {
      ++count;
   }
   if (checkNeighbor(bw, bh, x, y+1, cells) == 1)
   {
      ++count;
   }
   if (checkNeighbor(bw, bh, x+1, y+1, cells) == 1)
   {
      ++count;
   }
   return count;
}

void updateBoard(int bw, int bh, int currgen[bh][bw], int nextgen[bh][bw])
{
   int count = 0;
   for (int y = 0; y < bh; y++)
   {
      for (int x = 0; x < bw; x++)
      {
         count = countNeighbors(bw, bh, x, y, currgen);
         if (count == 3)
         {
            nextgen[y][x] = 1;
         }
         else if (count == 4)
         {
            nextgen[y][x] = currgen[y][x];
         }
         else
         {
            nextgen[y][x] = 0;
         }
      }
   }
}

int main(int argc, char* argv[])
{
   SDL_Window *win = NULL;
   SDL_Renderer *ren = NULL;
   int quit = 0;
   int boardWidth = SCREEN_WIDTH / RECT_WIDTH;
   int boardHeight = SCREEN_HEIGHT / RECT_HEIGHT;
   int switchGen = 0;
   int currgen[boardWidth][boardHeight];
   int nextgen[boardWidth][boardHeight];
   SDL_Event e;

   initSDL(&win, &ren);

   currgen[20][20] = 1;
   currgen[20][21] = 1;
   currgen[21][19] = 1;
   currgen[21][20] = 1;
   currgen[22][20] = 1;
   renderGame(ren, boardWidth, boardHeight, currgen);

   while (quit != 1)
   {
      while (SDL_PollEvent(&e))
      {
         if (e.type == SDL_QUIT)
         {
            quit = 1;
         }
      }
      if (switchGen == 0)
      {
         updateBoard(boardWidth, boardHeight, currgen, nextgen);
         renderGame(ren, boardWidth, boardHeight, nextgen);
         switchGen = 1;
      }
      else if (switchGen == 1)
      {
         updateBoard(boardWidth, boardHeight, nextgen, currgen);
         renderGame(ren, boardWidth, boardHeight, currgen);
         switchGen = 0;
      }
      SDL_Delay(200);
   }

   SDL_DestroyWindow(win);
   SDL_DestroyRenderer(ren);
   SDL_Quit();
   return 0;
}
