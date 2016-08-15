/**
  \file
  \brief This file contains network interaction inrerface.
*/

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdint.h>

#include "../include/client_server.h"

/**
  \brief Connect to server.
  \param server_ip Server ip address. Should be correct.

  Init all sockets, create logical connection to server and get unique id.
*/
int8_t Connect(char *server_ip);

/**
  \brief Handle player's action.
  \param action Player's action

  Send message to server.
*/
int8_t HandleAction(enum Doing action);

/**
  \brief Receive message from server.
  \param msg Pointer to pointer to msg.

  This func call malloc inside,
  so you should free memory after usage manually.
*/
int8_t RecvMsg(struct ServerToClient **msg); 

/**
  \brief Close all network connects.

  Close sockets.
*/
void ShutdownNetwork(void);

#endif /* _NETWORK_H_ */
