/*  Action table structure.
 *  \file action_table.h
 */

#ifndef _ACTION_TABLE_H_
#define _ACTION_TABLE_H_

#include <stdint.h>

#include "common.h"
#include "position.h"

/**/
struct ActionCell {
  uint8_t suicide;
  struct Position bomb_pos;
  struct Position move_pos;
}__attribute__((__packed__ ));

/*Structure that client send to server.*/
struct MsgToServer {
  uint8_t id;
  struct Position bomb_pos;
  struct Position move_pos;
}__attribute__((__packed__ ));

/*  Structure that server transfer to logic module. */
struct ActionTable {
  struct ActionCell player_info[MAX_PLAYER_AMOUNT];
};

#endif  /* _ACTION_TABLE_H_ */
