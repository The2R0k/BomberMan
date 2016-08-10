/* * * * * * * * * * * * * * * * * * * * * *
*  This file containts algorythm of        * 
*  field generation and imaging encourses  *
*  graphics.                               *
* * * * * * * * * * * * * * * * * * * * * */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

#define N 19 // size of field

/* Creating the devides of field */
void UpdateMatrix(int **matrix)
{
  for (int i = 0; i < N; i++){
    matrix[0][i] = 6; 
    matrix[i][0] = 6;
    matrix[N - 1][i] = 6;
    matrix[i][N - 1] = 6;
  }
}
/* Random generation */
void FillMatrix(int **matrix)
{
  for (int i = 1; i < N - 1; i++) {
    for(int j = 1; j < N - 1; j++){
      int n = rand() % 62;
      if (n > 60)
        matrix[i][j] = 5;   
    }
  }
}
/* prints graphics >.< */
void PrintGraphics(int **matrix)
{
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      if (matrix[i][j] == 0)
        printf("  ");
      else if (matrix[i][j] == 5)
        printf("o ");
      else if (matrix[i][j] == 6)
        printf("# ");
      else
        printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char const *argv[]) 
{
  srand(time(0)); //randomize

  struct Field field_loc;
/* # initializing field # */
  field_loc.location = malloc(sizeof(int *) * N);

  for (int i = 0; i < N; i++){
    field_loc.location[i] = malloc(sizeof(int) * N);

    for (int j = 0; j < N; j++){
      field_loc.location[i][j] = 0;
    }
  }
/* # end of initializing # */

  UpdateMatrix(field_loc.location);

/* # random fill */  
  for (int i = 0; i < 25; i++)
    FillMatrix(field_loc.location);
/* # end # */

  PrintGraphics(field_loc.location);

  free(field_loc.location);

  return 0;
}