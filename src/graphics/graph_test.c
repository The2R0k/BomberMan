/* * * * * * * * * * * * * * * * * * * * * *
*  This file containts algorythm of        * 
*  field generation and imaging            *
*  graphics.                               *
*  gs = graphic sourses, ms = menu state   *
* * * * * * * * * * * * * * * * * * * * * */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "./include/common.h"
#include "./include/bgraphics.h"

// All the game states
enum GameState { 
  MAINMENU = 0,
  GAME = 2,
  GAMEMENU = 1
};

/* Creating the devides of field */
void UpdateMatrix(enum Cell **matrix) {
  for (int i = 0; i < FIELD_SIZE; i++) {
    matrix[0][i] = 6; 
    matrix[i][0] = 6;
    matrix[FIELD_SIZE - 1][i] = 6;
    matrix[i][FIELD_SIZE - 1] = 6;
  }
}
/* Random generation */
void FillMatrix(enum Cell **matrix) {
  for (int i = 1; i < FIELD_SIZE - 1; i++) {
    for(int j = 1; j < FIELD_SIZE - 1; j++) {  
      int n = rand() % 62;
      if (n > 60)
        matrix[i][j] = 5;   
    }
  }
}
/* prints map to console */
void PrintGraphics(enum Cell **matrix) {
  for (int i = 0; i < FIELD_SIZE; i++) {
    for (int j = 0; j < FIELD_SIZE; j++) {
      if (matrix[i][j] == EMPTY)
        printf("  ");
      else if (matrix[i][j] == BOX)
        printf("o ");
      else if (matrix[i][j] == WALL)
        printf("# ");
      else if (matrix[i][j] == BOMB)
        printf("X ");
      else if (matrix[i][j] == FIRE)
        printf("x ");
      else
        printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char const *argv[]) {
  srand(time(0)); // randomize

  SDL_Event event; // even (for example keyboard)
  enum GameState game_state = MAINMENU; // current game state
  int ms = 1; // current menu position
  const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL); // keyboard state. Maniplating flags like SDL_Scancode_<KEY>

  struct Field field_loc; // game field
  struct Dklb dklb_loc; //game states
  struct GraphSources *gs = malloc(sizeof(struct GraphSources)); // graphic sourses
  GraphicsInit(gs); // initializing graphics

/* # initializing field # */
  field_loc.location = malloc(sizeof(enum Cell *) * FIELD_SIZE);
  for (int i = 0; i < FIELD_SIZE; i++) {
    field_loc.location[i] = malloc(sizeof(enum Cell) * FIELD_SIZE);
    for (int j = 0; j < FIELD_SIZE; j++) {
      field_loc.location[i][j] = 0;
    }
  }
/* # end of initializing # */

/* # test initializing of dklb# */
  dklb_loc.score = 0;
  dklb_loc.length = 1;
  dklb_loc.death = 0;
  dklb_loc.bomb = 0;

/* # index menu # */
  while (game_state == MAINMENU) {
    MenuShow(gs, game_state, ms);
    SDL_PollEvent(&event);
    SDL_PumpEvents();
    SDL_Delay(60);
    if ((keyboard_state[SDL_SCANCODE_UP]) && (ms > 1))
      ms--;
    if ((keyboard_state[SDL_SCANCODE_DOWN]) && (ms < 3))
      ms++;
    if (keyboard_state[SDL_SCANCODE_RETURN])
      switch (ms) {
        case 1:
          game_state = GAME;
          break;
        case 2:
          CleanGraph(gs);
          break;
        default:
          CleanGraph(gs);
          exit(0);
          break;
      }
  }
/* # # */

  UpdateMatrix(field_loc.location);

/* # random fill */  
  for (int i = 0; i < 25; i++)
    FillMatrix(field_loc.location);
/* # end # */

  PrintGraphics(field_loc.location);
  RefreshState(field_loc, dklb_loc, gs);

  game_state = GAME;
/* Game cycle */
  while (game_state != MAINMENU){
    RefreshState(field_loc, dklb_loc, gs);
    SDL_Delay(20);
    SDL_PollEvent(&event);
    SDL_PumpEvents();
    if(keyboard_state[SDL_SCANCODE_ESCAPE]) { // if ESC pressed - we see menu
      game_state = GAMEMENU;
      ms = 1; // refreshing menu position
      while (game_state == GAMEMENU) {
        MenuShow(gs, game_state, ms);
        SDL_Delay(20);
        SDL_PollEvent(&event);
        SDL_PumpEvents();
        if ((keyboard_state[SDL_SCANCODE_UP]) && (ms > 1)) // if key pressed "UP"
          ms--;
        if ((keyboard_state[SDL_SCANCODE_DOWN]) && (ms < 2)) // if key pressed "DOWN"
          ms++;
        if (keyboard_state[SDL_SCANCODE_RETURN]) // if key pressed "ENTER", switching between menu positions
          switch (ms) {
            case 1:
              game_state = GAME;
              break;
            default:
              game_state = MAINMENU;
              break;
          }
      }
    }
  } 
  CleanGraph(gs);

  free(field_loc.location);
  free(gs);

  return 0;
}