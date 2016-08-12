#ifndef _CLIENT_SERVER_H_
#define _CLIENT_SERVER_H_

/*struct for send udp message to server*/
/*first time sendig id: 0, bomb: 0;0, move 0;0*/
/*if server not full, server send new id*/
/*next time, must sending with that id*/

#include position.h
#include common.h

struct ClientToServer
{
  uint8_t id;
  uint8_t doing;
}__attribute__((__packed__ ));

/*struct to recv message from server*/
/*first time sending new id, and new field*/

struct ServerToClient
{
  uint8_t id;
  struct Field field;
  struct Dklb stats;
}__attribute__((__packed__ ));

#endif
