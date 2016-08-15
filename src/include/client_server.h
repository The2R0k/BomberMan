/**
  \file
  \brief File contains structs to provide client-server interaction.
*/

#ifndef _CLIENT_SERVER_H_
#define _CLIENT_SERVER_H_

#include "position.h"
#include "common.h"

/**
  \brief Enum to represent user actions.
*/
enum Doing {
  NOTHING = 0,
  PLANT_BOMB = 1,
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
  uint8_t id;
  enum Doing doing;
} __attribute__((packed));

/**
  \brief Struct to recv message from server.

  First time sending new id, and new field.
*/
struct ServerToClient {
  uint8_t id;
  struct Field field;
  struct Dklb stats;
} __attribute__((packed));

#endif /* _CLIENT_SERVER_H_ */
