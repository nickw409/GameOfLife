#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

#define RECT_WIDTH 20
#define RECT_HEIGHT 20
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

int logError(const char *err_msg);
void renderGame(SDL_Renderer *ren, int bw, int bh, int **cells);
void initSDL(SDL_Window **win, SDL_Renderer **ren);
int getNumRects(SDL_Renderer *ren);

#endif
