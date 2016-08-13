#ifndef _PLAYER_INFO_H_
#define _PLAYER_INFO_H_

#include "bomb_info.h"
#include "player_state.h"

struct PlayerInfo {
  enum PlayerState state;
  struct BombInfo bomb_info;
  struct Position pos;  /* Current player pos. */
  uint8_t suicide_time; /* No answer count. */
  uint8_t res_time;
};

#endif /* _PLAYER_INFO_H_ */
