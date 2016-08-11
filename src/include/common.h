#ifndef _COMMON_H_
#define _COMMON_H_

#define MAX_PLAYER_AMOUNT 4

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
};

/* death, kill, length counters & bomb flaq (have or not)*/
struct Dklb {
  uint8_t score;  /* How more players was killed*/
  uint8_t lenght; /* Length of fire. Up if player kill someone
                     and drop if player die*/
  uint8_t death;  /* How many death*/
  uint8_t bomb;   /* 1 if player have a bomb and 0 if don't have*/
};

#endif /* _COMMON_H_ */
