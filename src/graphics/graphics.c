/* * * * * * * * * * * * * * * * * * * * * *
*  This file containts graphic             * 
*  initialization and graphic processing   *
*  algorythms.                             *
* * * * * * * * * * * * * * * * * * * * * */
#include "../include/common.h"
#include "../include/bgraphics.h"

/* Adding elements to final image (before rendering) */
void ApplySurface(int x, int y, int h, int w, SDL_Texture *tex, SDL_Renderer *rend) {
  SDL_Rect pos;
  pos.x = x;
  pos.y = y;
  pos.h = h;
  pos.w = w;
  SDL_RenderCopy(rend, tex, NULL, &pos);
}

/* initializing graphic sources */
void GraphicsInit(struct GraphSources *gs) {
  int i = 0;
  /* Initializing graphic area */
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    exit(-1);
  }
  /* Initializing window */
  if ((gs->win = SDL_CreateWindow("BomberMan - online", 100, 100, 19 * PCSIZE + 200, 19 * PCSIZE, SDL_WINDOW_BORDERLESS)) == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    exit(-1);
  }
  /* Initializing render */
  if ((gs->ren = SDL_CreateRenderer(gs->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == NULL) { 
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    exit(-1);
  }
  /* # Adding interface texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/interface.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->interface = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
  /* # # */
  /* # Adding wall texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/wall.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }
  if((gs->wall = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
  /* # # */
  /* # Adding empty texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/empty.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->empty = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
  /* # # */
  /* # Adding box texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/box.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->box = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
  /* # # */
  /* # Adding players textures # */
  for (i = 0; i <= 3; i++) {
    char way_to_sourcefile[INTERMEDIATE_STR_SIZE];
    sprintf(way_to_sourcefile, "../img/player%d.bmp", i + 1);
    if ((gs->bmp = SDL_LoadBMP(way_to_sourcefile)) == NULL) {
      printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
      exit(-1);
    }

    if((gs->players[i] = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
      printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
      exit(-1);
    } 
    SDL_FreeSurface(gs->bmp);
  }
  /* # # */
  /* # Adding players with bomb textures # */
  for (i = 0; i <= 3; i++) {
    char way_to_sourcefile[INTERMEDIATE_STR_SIZE];
    sprintf(way_to_sourcefile, "../img/player%db.bmp", i + 1);
    if ((gs->bmp = SDL_LoadBMP(way_to_sourcefile)) == NULL) {
      printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
      exit(-1);
    }

    if((gs->players_b[i] = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
      printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
      exit(-1);
    } 
    SDL_FreeSurface(gs->bmp);
  }
  /* # # */
  /* # Adding bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/bomb.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->bomb = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
  /* # # */
  /* # Adding fire texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/fire.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->fire = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
  /* # # */
  /* # Adding score item texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/score.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->score = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
  /* # # */
  /* # Adding length inem texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/length.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->length = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
  /* # # */
  /* # Adding death item texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/death.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->death = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
  /* # # */
  /* # Adding (!)bomb ITEM texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/bombi.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->bombi = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
  /* # # */
  /* # Adding nums textures # */
  for (i = 0; i <= 9; i++) {
    char way_to_sourcefile[INTERMEDIATE_STR_SIZE];
    sprintf(way_to_sourcefile, "../img/%d.bmp", i);
    if ((gs->bmp = SDL_LoadBMP(way_to_sourcefile)) == NULL) {
      printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
      exit(-1);
    }

    if((gs->digits[i] = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
      printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
      exit(-1);
    } 
    SDL_FreeSurface(gs->bmp);
  }
  /* # # */
  SDL_RenderClear(gs->ren);
  SDL_RenderCopy(gs->ren, NULL, NULL, NULL);
  SDL_RenderPresent(gs->ren);  
}

/* prints interface items's values */
void PrintNums(int num, int y, struct GraphSources *gs) {
  int i = 0;
  char buffer[4];
  sprintf(buffer, "%d", num);
  printf("%s\n", buffer);
  for (i = 0; i < 3; i++) {
    if (buffer[i] != '\0')
      ApplySurface(735 + i * 10, y, 18, 10, gs->digits[buffer[i] - '0'], gs->ren);
    else
      break;
  }
}

/* Refreshing current game state*/
void RefreshState(struct Field *field_loc, struct Dklb *dklb_loc, struct GraphSources *gs) {
  int i = 0, j = 0;
  /* # Generating final image # */
  for (i = 0; i < FIELD_SIZE; i++) {
    for (j = 0; j < FIELD_SIZE; j++) {
      switch(field_loc->location[i][j]){
        case WALL:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->wall, gs->ren);
          break;
        case BOX:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->box, gs->ren);
          break;
        case BOMB:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->bomb, gs->ren);
          break;
        case PLAYER_1:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->players[0], gs->ren);
          break;
        case PLAYER_2:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->players[1], gs->ren);
          break;
        case PLAYER_3:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->players[2], gs->ren);
          break;
        case PLAYER_4:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->players[3], gs->ren);
          break;
        case FIRE:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->fire, gs->ren);
          break;
        case PLAYER_1_BOMB:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->players_b[0], gs->ren);
          break;
        case PLAYER_2_BOMB:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->players_b[1], gs->ren);
          break;
        case PLAYER_3_BOMB:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->players_b[2], gs->ren);
          break;
        case PLAYER_4_BOMB:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->players_b[3], gs->ren);
          break;
        default:
          ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->empty, gs->ren); 
          break;
      }
    }
  }

  /* Adding interface elements to final image */
  ApplySurface(646, 0, 646, 200, gs->interface, gs->ren);
  ApplySurface(666, 100, 30, 100, gs->score, gs->ren);
  PrintNums(dklb_loc->score, 106, gs);
  ApplySurface(666, 150, 30, 100, gs->length, gs->ren);
  PrintNums(dklb_loc->length, 156, gs);
  ApplySurface(666, 200, 30, 100, gs->death, gs->ren);
  PrintNums(dklb_loc->death, 206, gs);
  ApplySurface(666, 250, 30, 100, gs->bombi, gs->ren);
  PrintNums(dklb_loc->bomb, 256, gs);
  /* # end # */
  /* Presenting final image */
  SDL_RenderPresent(gs->ren);
}

void CleanGraph(struct GraphSources *gs) {
  int i = 0;
  /* Cleaning up */
  SDL_DestroyTexture(gs->wall);
  SDL_DestroyTexture(gs->empty);
  SDL_DestroyTexture(gs->box);
  SDL_DestroyTexture(gs->bomb);
  SDL_DestroyTexture(gs->fire);
  SDL_DestroyTexture(gs->interface);
  SDL_DestroyTexture(gs->interface);

  for (i = 0; i <= 9; i++) {
    SDL_DestroyTexture(gs->digits[i]);
  }
  for (i = 0; i <= 3; i++) {
    SDL_DestroyTexture(gs->players[i]);
  }
  for (i = 0; i <= 3; i++) {
    SDL_DestroyTexture(gs->players_b[i]);
  }

  SDL_DestroyRenderer(gs->ren);
  SDL_DestroyWindow(gs->win);
  SDL_Quit();
}
