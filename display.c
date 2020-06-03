#include "display.h"
#include <stdio.h>
#include <stdlib.h>

#define NGOL_VERSION "0.0.1"

void logError(char *err)
{
   fprintf(stderr, "%s: %s\n", err, SDL_GetError());
   exit(1);
}

void renderRect(SDL_Renderer *ren, int cellIdx)
{
   //This only works for square screens and rects
   SDL_Rect r;
   //find row loc using division
   r.x = RECT_WIDTH * (cellIdx % (SCREEN_WIDTH / RECT_WIDTH));
   //find col loc using modulus
   r.y = RECT_HEIGHT * (cellIdx / (SCREEN_HEIGHT / RECT_HEIGHT));
   r.w = RECT_WIDTH;
   r.h = RECT_HEIGHT;

   SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
   SDL_RenderFillRect(ren, &r);
   SDL_RenderPresent(ren);
}

void renderGame(SDL_Renderer *ren, int cells[], int len)
{
   SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
   SDL_RenderClear(ren);
   SDL_RenderPresent(ren);
   for (int i = 0; i < len; i++)
   {
      if (cells[i] == 1)
      {
         renderRect(ren, i);
      }
   }
}

void initSDL(SDL_Window **win, SDL_Renderer **ren)
{
   if (SDL_Init(SDL_INIT_VIDEO) != 0)
   {
      logError("Error initializing SDL");
   }

   *win = SDL_CreateWindow(
               "Nick's Game of Life",
               SDL_WINDOWPOS_CENTERED,
               SDL_WINDOWPOS_CENTERED,
               SCREEN_WIDTH,
               SCREEN_HEIGHT,
               0
   );

   if (*win == NULL)
   {
      logError("Error creating window");
   }

   *ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);

   if (*ren == NULL)
   {
      logError("Error creating renderer");
   }
}

int getNumRects(SDL_Renderer *ren)
{
   int w = 0;
   int h = 0;
   if (SDL_GetRendererOutputSize(ren, &w, &h) == 1)
   {
      logError("Error getting window size");
   }
   return (w / RECT_WIDTH) * (h / RECT_HEIGHT);
}

int main(int argc, char* argv[])
{
   SDL_Window *win = NULL;
   SDL_Renderer *ren = NULL;
   int quit = 0;
   int len = 0;
   int *cells = NULL;
   SDL_Event e;

   initSDL(&win, &ren);

   SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
   SDL_RenderClear(ren);
   SDL_RenderPresent(ren);
   len = getNumRects(ren);
   cells = calloc(len, sizeof(int));
   cells[0] = 1;
   cells[6] = 1;
   cells[7] = 1;
   cells[15] = 1;
   cells[23] = 1;
   renderGame(ren, cells, len);

   printf("%d\n", getNumRects(ren));

   while (quit != 1)
   {
      SDL_PollEvent(&e);
      
      if (e.type == SDL_QUIT)
      {
         quit = 1;
      }
   }

   SDL_DestroyWindow(win);
   SDL_DestroyRenderer(ren);
   SDL_Quit();
   free(cells);
   return 0;
}
