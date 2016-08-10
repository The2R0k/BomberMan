/*  Action table structure.
 *  \file action_table.h
 */

#ifndef _ACTION_TABLE_H_
#define _ACTION_TABLE_H_

#include <stdint.h>

#include "commmon.h"
#include "position.h"

/**/
struct ActionCell {
  uint8_t suicide;
  struct Position bomb_pos;
  struct Position move_pos;
};

/*  Structure that server transfer to logic module. */
struct ActionTable {
  struct ActionCell player_info[MAX_PLAYER_AMOUNT];
};

#endif  /* _ACTION_TABLE_H_ */
