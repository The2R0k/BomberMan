#include "network.h"

#include "../include/action_table.h"
#include "../include/client_server.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <unistd.h>

#define SERVER_PORT 4444
#define CLIENT_PORT 9999

#define SUCCESS 1
#define FAIL 0

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
static int8_t InitUDP(char *server_ip);

/**
  \brief Init TCP socket.
*/
static int8_t InitTCP(void);

/**
  \brief Send first udp msg.
*/
static int8_t Registration(void);

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
int8_t InitUDP(char *server_ip) {
  if ((udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("socket() error");
    return FAIL;
  }

  memset(&server_addr, 0, sizeof(struct sockaddr_in));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  server_addr.sin_port = htons(SERVER_PORT);

  return SUCCESS;
}

int8_t InitTCP(void) {
  const int on = 1;
  socklen_t server_len;

  if ((tcp_listener_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket()");
    return FAIL;
  }

  memset(&client_addr, 0, sizeof(struct sockaddr_in));
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = INADDR_ANY;
  client_addr.sin_port = htons(CLIENT_PORT);

  if (setsockopt(tcp_listener_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                 &on, sizeof(on)) < 0) {
    perror("setsockopt() error");
    close(tcp_listener_sock);
    close(udp_sock);
    return FAIL;
  }

  if (bind(tcp_listener_sock, (struct sockaddr *) &client_addr,
           sizeof(client_addr)) < 0) {
    perror("bind() error");
    close(tcp_listener_sock);
    close(udp_sock);
    return FAIL;
  }

  if (listen(tcp_listener_sock, 1) < 0) {
    perror("listen() error");
    close(tcp_listener_sock);
    close(udp_sock);
    return FAIL;
  }

  server_len = sizeof(server_addr);
  if((client_sock = accept(tcp_listener_sock, (struct sockaddr*)&server_addr,
                           &server_len)) < 0) {
    perror("accept() error");
    close(udp_sock);
    return FAIL;
  }

  return SUCCESS;
}

int8_t Registration(void) {
  struct ClientToServer msg;

  msg.id = 0;
  msg.doing = NOTHING;
  if (!SendMsg(&msg)) {
    /* TODO: add error handling. Server may be unreachable. */
    return FAIL;
  }

  return SUCCESS;
}

int8_t SendMsg(struct ClientToServer *msg) {
  if (sendto(udp_sock, msg, sizeof(struct ClientToServer), 0,
        (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    /* TODO: add net errors such as disconnect. */
    perror("send()");
    close(udp_sock);
    return FAIL;
  }
  return SUCCESS;
}

/*============================*/
/*                            */
/* Interface definition.      */
/*                            */
int8_t Connect(char *server_ip) {
  struct ServerToClient *msg_with_id;
  
  player_id = 0;
  if (!InitUDP(server_ip))
    return FAIL;
  if (!Registration())
    return FAIL;
  if (!InitTCP())
    return FAIL;
  printf("Tcp connected\n");

  if (!RecvMsg(&msg_with_id))
    return FAIL;

  player_id = msg_with_id->id;
  free(msg_with_id);
  msg_with_id = NULL;

  printf("Connect done\n");

  return SUCCESS; 
}

int8_t HandleAction(enum Doing action) {
  struct ClientToServer msg;

  msg.id = player_id;
  msg.doing = action;

  if (!SendMsg(&msg)) {
    perror("HandleAction() error");
    return FAIL;
  }
  return SUCCESS;
}

int8_t RecvMsg(struct ServerToClient **msg) {
  int bytes;

  *msg = malloc(sizeof(struct ServerToClient));
  (*msg)->id = 0;
  if ((bytes = recv(client_sock, (*msg),
                    sizeof(struct ServerToClient), 0)) <= 0) {
    perror("recv()");
    ShutdownNetwork();
    return FAIL;
  }
  printf("Bytes recv: %d\n", bytes);
  return SUCCESS;
}

void ShutdownNetwork(void) {
  close(tcp_listener_sock);
  close(udp_sock);
  close(client_sock);
}
