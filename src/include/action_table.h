/**
  \file action_table.h
  \brief Action table structure.
*/

#ifndef _ACTION_TABLE_H_
#define _ACTION_TABLE_H_

#include <stdint.h>

#include "common.h"
#include "position.h"

struct ActionCell {
  struct Position move_pos;
  uint8_t suicide;
  uint8_t bomb;
}__attribute__((packed));

/* Structure that server transfer to logic module. */
struct ActionTable {
  struct ActionCell player_info[MAX_PLAYER_AMOUNT];
};

#endif /* _ACTION_TABLE_H_ */