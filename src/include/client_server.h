#ifndef _CLIENT_SERVER_H_
#define _CLIENT_SERVER_H_

#include "position.h"
#include "common.h"

enum Doing {
  NOTHING = 0,
  BOMB = 1,
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_TOP,
  MOVE_DOWN
};

/* Struct for send udp message to server
 * first time sending: id=0, bomb={0;0}, move={0;0}.
 * If server is not full, server send new id
 * next time, must sending with that id.
 */
 struct ClientToServer {
  uint8_t id;
  enum Doing doing;
} __attribute__((packed));

/* Struct to recv message from server.
 * First time sending new id and new field.
 */
struct ServerToClient {
  uint8_t id;
  struct Field field;
  struct Dklb stats;
} __attribute__((packed));

#endif /* _CLIENT_SERVER_H_ */
