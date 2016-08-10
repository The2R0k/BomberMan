#ifndef _COMMON_H_
#define _COMMON_H_

enum Cell {
  EMPTY = 0,
  PLAYER_1 = 1,
  PLAYER_2,
  PLAYER_3,
  PLAYER_4,
  BOX,
  WALL,
  BOMB,
  FIRE
};

struct field{
  int **location;
  int score;
  int lenght;
  int death;
};

#endif /* _COMMON_H_ */
