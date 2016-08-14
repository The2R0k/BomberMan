/**
  \file
  \brief File contains structs to provide client-server interaction.
*/

#ifndef _CLIENT_SERVER_H_
#define _CLIENT_SERVER_H_

/* Struct for send udp message to server
 * first time sendig id: 0, bomb: 0;0, move 0;0
 * if server not full, server send new id
 * next time, must sending with that i
 */

#include "position.h"
#include "common.h"

/**
  \brief Enum to represent user actions.
*/
enum Doing {
  NOTHING = 0,
  BOMB = 1,
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_TOP,
  MOVE_DOWN
};

/**
  \brief This struct should be sent to server from client.
*/
struct ClientToServer
{
  enum Doing doing;
  uint8_t id;
} __attribute__((packed));

/**
  \briefStruct to recv message from server.

  First time sending new id, and new field.
*/
struct ServerToClient {
  struct Field field;
  struct Dklb stats;
  uint8_t id;
} __attribute__((packed));


#endif
