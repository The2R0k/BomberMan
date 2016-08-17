#ifndef _BGRAPHICS_H_
#define _BGRAPHICS_H_

#include <SDL2/SDL.h>

struct GraphSources {
  SDL_Window *win; /* Window for graphics */
  SDL_Renderer *ren; /* Render */
  SDL_Surface *bmp; /* Bitmap image */
  SDL_Texture *wall, *empty, *box, *bomb, *interface,
  *player_one, *player_two, *player_three, *player_four, *fire,
  *player_one_b, *player_two_b, *player_three_b, *player_four_b; /* Sources */ 
  SDL_Texture *menuNG, *menuJN, *menuRS, *menuEX, *menuNGC, *menuJNC, *menuRSC, *menuEXC; /* menu items */
  SDL_Texture *score, *length, *death, *bombi; /* interface items */
  SDL_Texture *one, *two, *three, *four, *five, *six, *seven, *eight, *nine, *zero; /* interface symbols */
};

/* prototypes of graphic functions */
void RefreshState(struct Field *, struct Dklb *, struct GraphSources *); /* showing current state of monitor */
void GraphicsInit(struct GraphSources *); /* Initializatin graphics (window, render etc) */
void MenuShow(struct GraphSources *, int, int); /* Showing index and game menu 
                                                   first int arg - current gamestate, 
                                                   seckond - current player position in menu   */
void CleanGraph(struct GraphSources *); /* Cleainig up all the graphics */
void PrintNums(int, int, struct GraphSources *); /* Prints values of DKLB. 1st arg - num to print, second arg - y position; */

/* graphics constants */
#define PCSIZE 34 /* bitmap block size */

#endif /* _BGRAPHICS_H_ */