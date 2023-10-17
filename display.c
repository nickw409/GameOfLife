#include "display.h"
#include <stdio.h>
#include <stdlib.h>

int logError(const char *error_msg)
{
   const char *sdl_error = SDL_GetError();

   if (sdl_error[0] == '\0')
   {
      return 0;
   }
   fprintf(stderr, "%s: %s\n", error_msg, sdl_error);
   SDL_ClearError();
   return 1;
}

void renderRect(SDL_Renderer *ren, int x, int y)
{
   //This only works for square screens and rects
   SDL_Rect r;
   r.x = x * RECT_WIDTH; 
   r.y = y * RECT_HEIGHT;
   r.w = RECT_WIDTH;
   r.h = RECT_HEIGHT;

   if (SDL_SetRenderDrawColor(ren, 0, 0, 0, 255) != 0)
   {
      logError("Error setting draw color");
   }
   if (SDL_RenderFillRect(ren, &r))
   {
      logError("Error rendering rect");
   }
}

void renderGame(SDL_Renderer *ren, int bw, int bh, int **cells)
{
   if (SDL_SetRenderDrawColor(ren, 255, 255, 255, 255) != 0)
   {
      logError("Error setting draw color");
   }
   if (SDL_RenderClear(ren) != 0)
   {
      logError("Error calling SDL_RenderClear");
   }
   SDL_RenderPresent(ren);

   for (int i = 0; i < bh; i++)
   {
      for (int j = 0; j < bw; j++)
      {
         if (cells[i][j] == 1)
         {
            renderRect(ren, i, j);
         }
      }
   }
   SDL_RenderPresent(ren);
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
