/* * * * * * * * * * * * * * * * * * * * * *
*  This file containts graphic             * 
*  initialization and graphic processing   *
*  algorythms.                             *
* * * * * * * * * * * * * * * * * * * * * */
#include <SDL2/SDL.h>
#include "./include/common.h"
#include "./include/bgraphics.h"
/* Adding elements to final image */
void ApplySurface(int x, int y, int h, int w, SDL_Texture *tex, SDL_Renderer *rend) {
  SDL_Rect pos;
  pos.x = x;
  pos.y = y;
  pos.h = h;
  pos.w = w;
  SDL_RenderCopy(rend, tex, NULL, &pos);
}

struct GraphSources GraphicsInit() {
  struct GraphSources gs;
/* Initializing graphic area */
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
    printf("SDL_Init Error: %s\n", SDL_GetError());
    exit(-1);
  }
/* Initializing window */
  if ((gs.win = SDL_CreateWindow("BomberMan - online", 100, 100, 19 * PCSIZE + 200, 19 * PCSIZE, SDL_WINDOW_SHOWN)) == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    exit(-1);
  }
/* Initializing render */
  if ((gs.ren = SDL_CreateRenderer(gs.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == NULL) { 
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    exit(-1);
  }

/* # Adding interface texture # */
  if ((gs.bmp = SDL_LoadBMP("./img/interface.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs.interface = SDL_CreateTextureFromSurface(gs.ren, gs.bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs.bmp);
/* # # */

/* # Adding wall texture # */
  if ((gs.bmp = SDL_LoadBMP("./img/wall.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs.wall = SDL_CreateTextureFromSurface(gs.ren, gs.bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs.bmp);
/* # # */

/* # Adding empty texture # */
  if ((gs.bmp = SDL_LoadBMP("./img/empty.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs.empty = SDL_CreateTextureFromSurface(gs.ren, gs.bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs.bmp);
/* # # */

/* # Adding box texture # */
  if ((gs.bmp = SDL_LoadBMP("./img/box.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs.box = SDL_CreateTextureFromSurface(gs.ren, gs.bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs.bmp);
/* # # */

/* # Adding player one texture # */
  if ((gs.bmp = SDL_LoadBMP("./img/player1.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs.player_one = SDL_CreateTextureFromSurface(gs.ren, gs.bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs.bmp);
/* # # */

/* # Adding player two texture # */
  if ((gs.bmp = SDL_LoadBMP("./img/player2.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs.player_two = SDL_CreateTextureFromSurface(gs.ren, gs.bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs.bmp);
/* # # */

/* # Adding player three texture # */
  if ((gs.bmp = SDL_LoadBMP("./img/player3.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs.player_three = SDL_CreateTextureFromSurface(gs.ren, gs.bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs.bmp);
/* # # */

/* # Adding player four texture # */
  if ((gs.bmp = SDL_LoadBMP("./img/player4.bmp")) == NULL) { 
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs.player_four = SDL_CreateTextureFromSurface(gs.ren, gs.bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs.bmp);
/* # # */


/* # Adding bomb texture # */
  if ((gs.bmp = SDL_LoadBMP("./img/bomb.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs.bomb = SDL_CreateTextureFromSurface(gs.ren, gs.bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs.bmp);
/* # # */

/* # Adding bomb texture # */
  if ((gs.bmp = SDL_LoadBMP("./img/fire.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs.fire = SDL_CreateTextureFromSurface(gs.ren, gs.bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs.bmp);
/* # # */

  SDL_RenderClear(gs.ren);
  SDL_RenderCopy(gs.ren, NULL, NULL, NULL);
  SDL_RenderPresent(gs.ren);  

  return gs;
}

void ShowCurrentBar(struct Field field_loc, struct Dklb dklb_loc, struct GraphSources gs) {
/* # Generating final image # */
  for (int i = 0; i < FIELDSIZE; i++) {
    for (int j = 0; j < FIELDSIZE; j++) {
      if (field_loc.location[i][j] == WALL)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs.wall, gs.ren);
      else if (field_loc.location[i][j] == BOX)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs.box, gs.ren);
      else if (field_loc.location[i][j] == BOMB)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs.bomb, gs.ren);
      else if (field_loc.location[i][j] == PLAYER_1)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs.player_one, gs.ren);
      else if (field_loc.location[i][j] == PLAYER_2)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs.player_two, gs.ren);
      else if (field_loc.location[i][j] == PLAYER_3)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs.player_three, gs.ren);
      else if (field_loc.location[i][j] == PLAYER_4)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs.player_four, gs.ren);
      else if (field_loc.location[i][j] == FIRE)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs.fire, gs.ren);
      else
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs.empty, gs.ren); 
    }
  }

  ApplySurface(646, 0, 646, 200, gs.interface, gs.ren);
/* # end # */
/* Presenting final image */
  SDL_RenderPresent(gs.ren);
  SDL_Delay(1500); // ttl of window (!remove this - to do!)
}

struct GraphSources CleanGraph(struct GraphSources gs) {
  /* Cleaning up */
  SDL_DestroyTexture(gs.wall);
  SDL_DestroyTexture(gs.empty);
  SDL_DestroyTexture(gs.box);
  SDL_DestroyTexture(gs.bomb);
  SDL_DestroyTexture(gs.fire);
  SDL_DestroyTexture(gs.interface);
  SDL_DestroyTexture(gs.player_one);
  SDL_DestroyTexture(gs.player_two);
  SDL_DestroyTexture(gs.player_three);
  SDL_DestroyTexture(gs.player_four);

  SDL_DestroyRenderer(gs.ren);
  SDL_DestroyWindow(gs.win);
  SDL_Quit();

  return gs;
}