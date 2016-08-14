#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>

#define MAX_PLAYER_AMOUNT 4
#define FIELD_SIZE        19

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
  FIRE, /*  Fire effect after detonating. */
  PLAYER_1_BOMB,
  PLAYER_2_BOMB,
  PLAYER_3_BOMB,
  PLAYER_4_BOMB
};

struct Field {
  enum Cell **location;
};

/*  Death, kill, length counters & bomb flag (have or not). */
struct Dklb {
  uint8_t score;  /*  How more players was killed. */
  uint8_t length; /*  Length of fire. Increment if player kills someone
                      and drops if player die. */
  uint8_t death;  /*  How many death. */
  uint8_t bomb;   /*  1 if player has a bomb and 0 if hasn't. */
};

#endif /* _COMMON_H_ */
