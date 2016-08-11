/* * * * * * * * * * * * * * * * * * * * * *
*  This file containts algorythm of        * 
*  field generation and imaging            *
*  graphics.                               *
* * * * * * * * * * * * * * * * * * * * * */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "./include/common.h"
#include "./include/bgraphics.h"

/* Creating the devides of field */
void UpdateMatrix(enum Cell **matrix) {
  for (int i = 0; i < FIELDSIZE; i++) {
    matrix[0][i] = 6; 
    matrix[i][0] = 6;
    matrix[FIELDSIZE - 1][i] = 6;
    matrix[i][FIELDSIZE - 1] = 6;
  }
}
/* Random generation */
void FillMatrix(enum Cell **matrix) {
  for (int i = 1; i < FIELDSIZE - 1; i++) {
    for(int j = 1; j < FIELDSIZE - 1; j++) {  
      int n = rand() % 62;
      if (n > 60)
        matrix[i][j] = 5;   
    }
  }
}
/* prints graphics >.< */
void PrintGraphics(enum Cell **matrix) {
  for (int i = 0; i < FIELDSIZE; i++) {
    for (int j = 0; j < FIELDSIZE; j++) {
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

  struct Field field_loc;
  struct Dklb dklb_loc;
  struct GraphSources gs = GraphicsInit();

/* # initializing field # */
  field_loc.location = malloc(sizeof(enum Cell *) * FIELDSIZE);

  for (int i = 0; i < FIELDSIZE; i++) {
    field_loc.location[i] = malloc(sizeof(enum Cell) * FIELDSIZE);

    for (int j = 0; j < FIELDSIZE; j++) {
      field_loc.location[i][j] = 0;
    }
  }
/* # end of initializing # */

  UpdateMatrix(field_loc.location);

/* # random fill */  
  for (int i = 0; i < 25; i++)
    FillMatrix(field_loc.location);
/* # end # */

  field_loc.location[14][12] = BOMB;
  field_loc.location[12][3] = PLAYER_1;
  field_loc.location[13][3] = PLAYER_2;
  field_loc.location[15][8] = PLAYER_3;
  field_loc.location[5][12] = PLAYER_4;
  PrintGraphics(field_loc.location);

  ShowCurrentBar(field_loc, dklb_loc, gs);

  field_loc.location[14][12] = FIRE;
  field_loc.location[12][3] = EMPTY;
  field_loc.location[13][3] = EMPTY;
  field_loc.location[15][8] = EMPTY;
  field_loc.location[5][12] = EMPTY;
  field_loc.location[1][3] = PLAYER_1;
  field_loc.location[1][4] = PLAYER_2;
  field_loc.location[1][5] = PLAYER_3;
  field_loc.location[1][6] = PLAYER_4;
  ShowCurrentBar(field_loc, dklb_loc, gs);
  gs = CleanGraph(gs);

  free(field_loc.location);

  return 0;
}