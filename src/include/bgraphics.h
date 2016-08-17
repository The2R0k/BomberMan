/**
  \file
  \brief This file contains interface to interact with graphics.
*/

#ifndef _BGRAPHICS_H_
#define _BGRAPHICS_H_

#include <SDL2/SDL.h>

/* graphics constants */
#define PCSIZE 34 /* bitmap block size */
#define INTERMEDIATE_STR_SIZE 64 /* Size of ways to omg directory */

struct GraphSources {
  SDL_Window *win; /* Window for graphics */
  SDL_Renderer *ren; /* Render */
  SDL_Surface *bmp; /* Bitmap image */
  SDL_Texture *wall, *empty, *box, *bomb, *interface, *fire;
  SDL_Texture *players[4], *players_b[4]; /* Players's bitmaps */ 
  SDL_Texture *digits[10]; /* interface symbols */
  /* Sources */ 
  SDL_Texture *menuNG, *menuJN, *menuRS, *menuEX, *menuNGC, *menuJNC,
              *menuRSC, *menuEXC; /* menu items */
  SDL_Texture *score, *length, *death, *bombi; /* interface items */
};

/* prototypes of graphic functions */
void RefreshState(struct Field *, struct Dklb *, struct GraphSources *); /* showing current state of monitor */
void GraphicsInit(struct GraphSources *); /* Initializatin graphics (window, render etc) */
void CleanGraph(struct GraphSources *); /* Cleainig up all the graphics */
void PrintNums(int, int, struct GraphSources *); /* Prints values of DKLB. 1st arg - num to print, second arg - y position; */


#endif /* _BGRAPHICS_H_ */
