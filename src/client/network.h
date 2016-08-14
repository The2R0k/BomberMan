/**
  \file
  \brief This file contains network interaction inrerface.
*/

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdint.h>

/**
  \brief Connect to server.
  \param server_ip Server ip address. Should be correct.

  Init all sockets, create logical connection to server and get unique id.
*/
int8_t Connect(char *server_ip);

#endif /* _NETWORK_H_ */
