#include "game.h"

GameState checkDeadGame(int **lastgen, int **nextgen, int width, int height)
{
   int outer_loop, inner_loop;
   GameState state = DEAD;

   for (outer_loop = 0; outer_loop < width; outer_loop++)
   {
      for (inner_loop = 0; inner_loop < height; inner_loop++)
      {
         if (lastgen[outer_loop][inner_loop] != nextgen[outer_loop][inner_loop])
         {
            state = ALIVE;
         }
      }
   }

   return state;
}

int checkNeighbor(int bw, int bh, int x, int y, int **cells)
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

int countNeighbors(int bw, int bh, int x, int y, int **cells)
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

void destroyBoard(int **board, int width)
{
   int index;

   for (index = 0; index < width; index++)
   {
      free(board[index]);
   }

   free(board);
}

int **initBoard(int width, int height)
{
   int index;
   int **board = (int **)malloc(width*sizeof(int*));

   for (index = 0; index < width; index++)
   {
      board[index] = (int *)malloc(height*sizeof(int));
   }

   return board;
}

int runGame(SDL_Renderer *ren)
{
   SDL_Event e;
   GameState state = ALIVE;
   int quit = 0;
   int boardWidth = SCREEN_WIDTH / RECT_WIDTH;
   int boardHeight = SCREEN_HEIGHT / RECT_HEIGHT;
   int **lastgen = initBoard(boardWidth, boardHeight);
   int **currgen = initBoard(boardWidth, boardHeight);
   int **nextgen = initBoard(boardWidth, boardHeight);

   setupBoard(currgen, boardWidth, boardHeight);

   while (quit != 1 && state == ALIVE)
   {
      while (SDL_PollEvent(&e))
      {
         if (e.type == SDL_QUIT)
         {
            quit = 1;
         }
      }

      updateBoard(boardWidth, boardHeight, lastgen, currgen, nextgen);
      renderGame(ren, boardWidth, boardHeight, nextgen);
      state = checkDeadGame(lastgen, nextgen, boardWidth, boardHeight);
      SDL_Delay(50);
   }

   printf("Ending Game\n");
   destroyBoard(currgen, boardWidth);
   destroyBoard(nextgen, boardWidth);
   return quit;
}

void setupBoard(int **board, int width, int height)
{
   int rand_num = 0;
   int inner_loop, outer_loop;

   for (outer_loop = 0; outer_loop < width; outer_loop++)
   {
      for (inner_loop = 0; inner_loop < height; inner_loop++)
      {
         rand_num = rand() % 2;
         board[outer_loop][inner_loop] = rand_num;
      }
   }
}

void updateBoard(int bw, int bh, int **lastgen, int **currgen, int **nextgen)
{
   int count = 0;
   for (int y = 0; y < bh; y++)
   {
      for (int x = 0; x < bw; x++)
      {
         lastgen[y][x] = currgen[y][x];
         currgen[y][x] = nextgen[y][x];
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
   int quit_flag = 0, total_games = 0;

   //initialize SDL and game board, and seed rand, 
   initSDL(&win, &ren);
   srand(time(NULL));
   
   while (!quit_flag)
   {
      printf("Starting A New game\n");
      quit_flag = runGame(ren);
      total_games++;
      printf("Total Games: %d\n", total_games);
   }

   SDL_DestroyWindow(win);
   SDL_DestroyRenderer(ren);
   SDL_Quit();
   return 0;
}
