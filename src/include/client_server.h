/**
  \file
  \brief File contains structs to provide client-server interaction.
*/

#ifndef _CLIENT_SERVER_H_
#define _CLIENT_SERVER_H_

#include "position.h"
#include "common.h"

#define CLIENT_MSG_SIZE sizeof(struct ClientToServer)
#define SERVER_MSG_SIZE sizeof(struct ServerToClient)

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
  
  First sending: id=0, move={0;0}, bomb={0}
  if server not full, server send new id
  next time, must sending with that i
*/
struct ClientToServer {
  uint8_t id;
  enum Doing doing;
} __attribute__((packed));

/**
  \briefStruct to recv message from server.

  First sending: new id and new field.
*/
struct ServerToClient {
  uint8_t id;
  struct Field field;
  struct Dklb stats;
} __attribute__((packed));


#endif
