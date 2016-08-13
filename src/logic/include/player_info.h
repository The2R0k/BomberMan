#ifndef _PLAYER_INFO_H_
#define _PLAYER_INFO_H_

#include <stdint.h>

#include "bomb_info.h"
#include "player_state.h"

struct PlayerInfo {
  enum PlayerState state;
  struct BombInfo bomb_info;
  struct Position pos;  /* Current player pos. */
  int8_t suicide_time; /* No answer count. */
  int8_t res_time;
};

#endif /* _PLAYER_INFO_H_ */
