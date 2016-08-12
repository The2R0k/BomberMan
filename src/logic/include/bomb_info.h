#ifndef _BOMB_INFO_H_
#define _BOMB_INFO_H_

#include "../../include/position.h"

struct BombInfo {
  struct Position pos;
  uint8_t d_time;  /* Detonate time. */
};

#endif /* _BOMB_INFO_H_ */
