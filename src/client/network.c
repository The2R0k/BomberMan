#include "network.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <unistd.h>


#include "../include/action_table.h"
#include "../include/client_server.h"

#define SERVER_PORT 4444
#define CLIENT_PORT 9999

/*============================*/
/*                            */
/* Global variables.          */
/*                            */
static int tcp_listener_sock,
           udp_sock,
           client_sock;
static struct sockaddr_in client_addr,
                          server_addr;
static uint8_t player_id;

/*============================*/
/*                            */
/* Helpfull functions.        */
/*                            */

/**
  \brief Init UDP socket.
  \param server_ip Server ip address. Should be correct.
*/
static void InitUDP(char *server_ip);

/**
  \brief Init TCP socket.
*/
static void InitTCP(void);

/**
  \brief Close all network connects.
*/
static void ShutdownNetwork(void);

/**
  \brief Send first udp msg.
*/
static void Registration(void);

/**
  \brief Send message msg to server.
  \param msg Pointer to existing ClientToServer message.
  \return 1 on success, 0 otherwise.
*/
static int8_t SendMsg(struct ClientToServer *msg);

/*============================*/
/*                            */
/* Definitions.               */
/*                            */
void InitUDP(char *server_ip) {
  if ((udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("socket() error");
    exit(-1);
  }

  memset(&server_addr, 0, sizeof(struct sockaddr_in));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  server_addr.sin_port = htons(SERVER_PORT);
}

void InitTCP(void) {
  const int on = 1;
  socklen_t server_len;

  if ((tcp_listener_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket()");
    exit(-1);
  }

  memset(&client_addr, 0, sizeof(struct sockaddr_in));
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = INADDR_ANY;
  client_addr.sin_port = htons(CLIENT_PORT);

  if (setsockopt(tcp_listener_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                 &on, sizeof(on)) < 0) {
    perror("setsockopt() error");
    close(tcp_listener_sock);
    exit(-1);
  }

  if (bind(tcp_listener_sock, (struct sockaddr *) &client_addr,
           sizeof(client_addr)) < 0) {
    perror("bind() error");
    close(tcp_listener_sock);
    exit(-1);
  }

  if (listen(tcp_listener_sock, 1) < 0) {
    perror("listen() error");
    close(tcp_listener_sock);
    exit(-1);
  }

  server_len = sizeof(server_addr);
  if((client_sock = accept(tcp_listener_sock, (struct sockaddr*)&server_addr,
                           &server_len)) < 0) {
    perror("accept() error");
    exit(-1);
  }
}

void ShutdownNetwork(void) {
  close(tcp_listener_sock);
  close(udp_sock);
  close(client_sock);
}

void Registration(void) {
  struct ClientToServer msg;

  msg.id = 0;
  msg.doing = NOTHING;
  if (!SendMsg(&msg)) {
    /* TODO: add error handling. Server may be unreachable. */
  }
}

int8_t SendMsg(struct ClientToServer *msg) {
  if (sendto(udp_sock, msg, sizeof(struct ClientToServer), 0,
        (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    /* TODO: add net errors such as disconnect. */
    perror("send()");
    close(udp_sock);
    return 0;
  }
  return 1;
}

void RecvMsg(struct ServerToClient **msg) {
  int bytes;

  *msg = malloc(sizeof(struct ServerToClient));
  (*msg)->id = 0;
  if ((bytes = recv(client_sock, (*msg),
                    sizeof(struct ServerToClient), 0)) <= 0) {
    perror("recv()");
    close(client_sock);
    close(tcp_listener_sock);
    exit(-1);
  }
  printf("Bytes recv: %d\n", bytes);
}

/*============================*/
/*                            */
/* Interface definition.      */
/*                            */
int8_t Connect(char *server_ip) {
  struct ServerToClient *msg_with_id;
  
  player_id = 0;
  InitUDP(server_ip);
  Registration();
  InitTCP();
  printf("Tcp connected\n");

  RecvMsg(&msg_with_id);
  player_id = msg_with_id->id;
  free(msg_with_id);
  msg_with_id = NULL;

  printf("Connect done\n");

  return 1; 
}

int8_t HandleAction(enum Doing action) {
  struct ClientToServer msg;

  msg.id = player_id;
  msg.doing = action;

  if (!SendMsg(&msg)) {
    perror("HandleAction() error");
    return 0;
  }
  return 1;
}
