#include "game.h"

GameState checkGameState(Game *game)
{
   int outer_loop, inner_loop;
   GameState state = DEAD;

   for (outer_loop = 0; outer_loop < game->width; outer_loop++)
   {
      for (inner_loop = 0; inner_loop < game->height; inner_loop++)
      {
         if (game->lastgen[outer_loop][inner_loop] != 
                                          game->nextgen[outer_loop][inner_loop])
         {
            state = ALIVE;
         }
      }
   }

   return state;
}

int checkNeighbor(int bw, int bh, int x, int y, int **cells)
{
   if (x >= 0 && y >= 0 && x < bw && y < bh && cells[x][y] == 1)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

void copyBoard(int **dest, int **src, int width, int height)
{
   int width_idx, height_idx;

   for (width_idx = 0; width_idx < width; width_idx++)
   {
      for (height_idx = 0; height_idx < height; height_idx++)
      {
         dest[width_idx][height_idx] = src[width_idx][height_idx];
      }
   }
}

int countNeighbors(int bw, int bh, int x, int y, int **cells)
{
   int count = 0;
   int row, col;

   //Checking all 8 neighbors
   for (row = -1; row < 2; row++)
   {
      for (col = -1; col < 2; col++)
      {
         //Making sure cell is not current cell
         if (row != 0 || col != 0)
         {
            if (checkNeighbor(bw, bh, x+row, y+col, cells) == 1)
            {
               count++;
            }
         }
         
      }
   }

   return count;
}

int **destroyBoard(int **board, int width)
{
   int index;

   for (index = 0; index < width; index++)
   {
      free(board[index]);
   }

   free(board);
   return NULL;
}

Game *destroyGame(Game *game)
{
   game->lastgen = destroyBoard(game->lastgen, game->width);
   game->currgen = destroyBoard(game->currgen, game->width);
   game->nextgen = destroyBoard(game->nextgen, game->width);

   free(game);
   return NULL;
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

void populateBoard(int **board, int width, int height)
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

int runGame(SDL_Renderer *ren)
{
   SDL_Event e;
   Game *game = setupGame();
   int quit = 0;

   while (quit != 1 && game->state == ALIVE)
   {
      while (SDL_PollEvent(&e))
      {
         if (e.type == SDL_QUIT)
         {
            quit = 1;
         }
      }

      updateGame(game);
      renderGame(ren, game->width, game->height, game->nextgen);
      //Maybe change checkGameState to void
      game->state = checkGameState(game);
      SDL_Delay(200);
   }

   printf("Ending Game\n");
   game = destroyGame(game);
   return quit;
}

Game *setupGame()
{
   Game *game = (Game *)malloc(sizeof(Game));

   game->width = SCREEN_WIDTH / RECT_WIDTH;
   game->height = SCREEN_HEIGHT / RECT_HEIGHT;
   game->state = ALIVE;
   
   game->lastgen = initBoard(game->width, game->height);
   game->currgen = initBoard(game->width, game->height);
   populateBoard(game->currgen, game->width, game->height);
   game->nextgen = initBoard(game->width, game->height);
   copyBoard(game->nextgen, game->currgen, game->width, game->height);

   return game;
}

void updateGame(Game *game)
{
   int count = 0;
   int width_idx, height_idx;

   copyBoard(game->lastgen, game->currgen, game->width, game->height);
   copyBoard(game->currgen, game->nextgen, game->width, game->height);

   for (width_idx = 0; width_idx < game->width; width_idx++)
   {
      for (height_idx = 0; height_idx < game->height; height_idx++)
      {
         count = countNeighbors(game->width, game->height, width_idx, 
                                             height_idx, game->currgen);

         //Live cell
         if (game->currgen[width_idx][height_idx] == 1)
         {
            if (count == 2 || count == 3)
            {
               game->nextgen[width_idx][height_idx] = 1;
            }
            else
            {
               game->nextgen[width_idx][height_idx] = 0;
            }
         }
         //Dead cell
         else
         {
            if (count == 3)
            {
               game->nextgen[width_idx][height_idx] = 1;
            }
            else
            {
               game->nextgen[width_idx][height_idx] = 0;
            }
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
   logError("After destroy window");
   SDL_DestroyRenderer(ren);
   //logError("After destroy renderer");
   SDL_Quit();
   //logError("After sdl quit");
   return 0;
}
