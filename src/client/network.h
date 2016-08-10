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

void initNetwork();
void shutdownNetwork();
void sendMsg();
void recvMsg();

#endif /* _NETWORK_H_ */
