#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#define RECT_WIDTH 100
#define RECT_HEIGHT 100
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

void renderGame(SDL_Renderer *ren, int cells[], int len);

void initSDL(SDL_Window **win, SDL_Renderer **ren);

#endif
