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

/* # Adding player one texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/player1.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->player_one = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */

/* # Adding player one with bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/player1b.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->player_one_b = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */

/* # Adding player two texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/player2.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->player_two = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */

/* # Adding player two with bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/player2b.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->player_two_b = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */

/* # Adding player three texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/player3.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->player_three = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */

/* # Adding player three with bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/player3b.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->player_three_b = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */

/* # Adding player four texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/player4.bmp")) == NULL) { 
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->player_four = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */

/* # Adding player four with bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/player4b.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->player_four_b = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
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
/* # Adding num 0 texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/0.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->zero = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding num 1 texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/1.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->one = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding num 2 texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/2.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->two = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding num 3 texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/3.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->three = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding num 4 texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/4.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->four = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding num 5 texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/5.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->five = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding num 6 texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/6.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->six = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding num 7 texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/7.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->seven = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding num 8 texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/8.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->eight = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding num 9 texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/9.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->nine = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
  SDL_RenderClear(gs->ren);
  SDL_RenderCopy(gs->ren, NULL, NULL, NULL);
  SDL_RenderPresent(gs->ren);  
}

/* prints interface items's values */
void PrintNums(int num, int y, struct GraphSources *gs) {
  int i = 0;
  char *buffer = malloc(sizeof(char) * 4);
  bzero(buffer, sizeof(buffer));
  sprintf(buffer, "%d", num);
  printf("%s\n", buffer);
  for (i = 0; i < 3; i++) {
    switch (buffer[i]) {
      case '0':
        ApplySurface(735 + i * 10, y, 18, 10, gs->zero, gs->ren);
        break;
      case '1':
        ApplySurface(735 + i * 10, y, 18, 10, gs->one, gs->ren);
        break;
      case '2':
        ApplySurface(735 + i * 10, y, 18, 10, gs->two, gs->ren);
        break;
      case '3':
        ApplySurface(735 + i * 10, y, 18, 10, gs->three, gs->ren);
        break;
      case '4':
        ApplySurface(735 + i * 10, y, 18, 10, gs->four, gs->ren);
        break;
      case '5':
        ApplySurface(735 + i * 10, y, 18, 10, gs->five, gs->ren);
        break;
      case '6':
        ApplySurface(735 + i * 10, y, 18, 10, gs->six, gs->ren);
        break;
      case '7':
        ApplySurface(735 + i * 10, y, 18, 10, gs->seven, gs->ren);
        break;
      case '8':
        ApplySurface(735 + i * 10, y, 18, 10, gs->eight, gs->ren);
        break;
      case '9':
        ApplySurface(735 + i * 10, y, 18, 10, gs->nine, gs->ren);
        break;
      default:
        break;
    }
  }
  free(buffer);
}

/* Refreshing current game state*/
void RefreshState(struct Field *field_loc, struct Dklb *dklb_loc, struct GraphSources *gs) {
  int i = 0, j = 0;
/* # Generating final image # */
  for (i = 0; i < FIELD_SIZE; i++) {
    for (j = 0; j < FIELD_SIZE; j++) {
      if (field_loc->location[i][j] == WALL)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->wall, gs->ren);
      else if (field_loc->location[i][j] == BOX)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->box, gs->ren);
      else if (field_loc->location[i][j] == BOMB)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->bomb, gs->ren);
      else if (field_loc->location[i][j] == PLAYER_1)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->player_one, gs->ren);
      else if (field_loc->location[i][j] == PLAYER_2)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->player_two, gs->ren);
      else if (field_loc->location[i][j] == PLAYER_3)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->player_three, gs->ren);
      else if (field_loc->location[i][j] == PLAYER_4)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->player_four, gs->ren);
      else if (field_loc->location[i][j] == FIRE)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->fire, gs->ren);
      else if (field_loc->location[i][j] == PLAYER_1_BOMB)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->player_one_b, gs->ren);
      else if (field_loc->location[i][j] == PLAYER_2_BOMB)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->player_two_b, gs->ren);
      else if (field_loc->location[i][j] == PLAYER_3_BOMB)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->player_three_b, gs->ren);
      else if (field_loc->location[i][j] == PLAYER_4_BOMB)
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->player_four_b, gs->ren);
      else
        ApplySurface(j * PCSIZE, i * PCSIZE, PCSIZE, PCSIZE, gs->empty, gs->ren); 
    }
  }

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
  /* Cleaning up */
  SDL_DestroyTexture(gs->wall);
  SDL_DestroyTexture(gs->empty);
  SDL_DestroyTexture(gs->box);
  SDL_DestroyTexture(gs->bomb);
  SDL_DestroyTexture(gs->fire);
  SDL_DestroyTexture(gs->interface);
  SDL_DestroyTexture(gs->player_one);
  SDL_DestroyTexture(gs->player_two);
  SDL_DestroyTexture(gs->player_three);
  SDL_DestroyTexture(gs->player_four);
  SDL_DestroyTexture(gs->interface);

  SDL_DestroyRenderer(gs->ren);
  SDL_DestroyWindow(gs->win);
  SDL_Quit();
}

/* Showing Main and Game menu */
void MenuShow(struct GraphSources *gs, int gamestate, int menu_state) { 
  /* # Adding bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/NewGame.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->menuNG = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/NewGameC.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->menuNGC = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/Join.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->menuJN = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/JoinC.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->menuJNC = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/Resume.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->menuRS = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/ResumeC.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->menuRSC = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/Exit.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->menuEX = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */
/* # Adding bomb texture # */
  if ((gs->bmp = SDL_LoadBMP("../img/ExitC.bmp")) == NULL) {
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    exit(-1);
  }

  if((gs->menuEXC = SDL_CreateTextureFromSurface(gs->ren, gs->bmp)) == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    exit(-1);
  } 
  SDL_FreeSurface(gs->bmp);
/* # # */

if (gamestate == 0) {
  if (menu_state != 1)
    ApplySurface(220, 200, 50, 400, gs->menuNG, gs->ren);
  else
    ApplySurface(220, 200, 50, 400, gs->menuNGC, gs->ren);

  if (menu_state != 2)
    ApplySurface(220, 270, 50, 400, gs->menuJN, gs->ren);
  else
    ApplySurface(220, 270, 50, 400, gs->menuJNC, gs->ren);

  if (menu_state != 3)
    ApplySurface(220, 340, 50, 400, gs->menuEX, gs->ren);
  else
    ApplySurface(220, 340, 50, 400, gs->menuEXC, gs->ren);
} else {
  if (menu_state != 1)
    ApplySurface(220, 200, 50, 400, gs->menuRS, gs->ren);
  else
    ApplySurface(220, 200, 50, 400, gs->menuRSC, gs->ren);
  if (menu_state != 2)
    ApplySurface(220, 270, 50, 400, gs->menuEX, gs->ren);
  else
    ApplySurface(220, 270, 50, 400, gs->menuEXC, gs->ren);
  }
  SDL_RenderPresent(gs->ren);
}
