#ifndef _COMMON_H_
#define _COMMON_H_

/*  Enum to represent a cell of game field. */
enum Cell {
  EMPTY = 0,
  PLAYER_1 = 1,
  PLAYER_2,
  PLAYER_3,
  PLAYER_4,
  BOX,  /*  Can be destroyed. */
  WALL, /*  Can not be destroyed.*/
  BOMB,
  FIRE  /*  Fire effect after detonating. */
};

struct Field {
  int **location;
  int score;
  int lenght;
  int death;
};

#endif /* _COMMON_H_ */
