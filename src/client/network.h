#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <malloc.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "../include/action_table.h"
#include "../include/client_server.h"

#define CONNECT_NEW_PLAYER 0
#define PLAYER_DID_ACTION 1
#define PORT 9999

/*init and shutdown network*/
void initUDP(char const *argv[]);
void initTCP();
void shutdownNetwork();
/*send position info...
  when connect new player */
void registration(struct ClientToServer *msg);
/*when player did action */
void sendMsg(struct ClientToServer *msg);
/*get message from server*/
struct ServerToClient * recvMsg();

/*for debug*/
struct ClientToServer * recvMsg2();

#endif /* _NETWORK_H_ */
