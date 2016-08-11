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
#include "../include/action_table.h"

#define CONNECT_NEW_PLAYER 0
#define PLAYER_DID_ACTION 1

void initNetwork(char const *argv[]);
void shutdownNetwork();
void sendMsg(struct MsgToServer *msg);
char recvMsg();

#endif /* _NETWORK_H_ */
