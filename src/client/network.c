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

static void SendMsg(struct ClientToServer *msg);

static struct ServerToClient RecvMsg(void);

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
}

int8_t Connect(char *server_ip) {
  struct ServerToClient msg_with_id;
  
  player_id = 0;

  printf("Init udp\n");
  InitUDP(server_ip);
  printf("Registration\n");
  Registration();
  printf("Init tcp\n");
  InitTCP();
  printf("Done tcp connection\n");

  msg_with_id = RecvMsg();
  player_id = msg_with_id.id;
   
  return 1; 
}

void Registration(void) {
  struct ClientToServer msg;

  msg.id = 0;
  msg.doing = NOTHING;
  SendMsg(&msg); 
}

void SendMsg(struct ClientToServer *msg) {
  if (sendto(udp_sock, msg, CLIENT_MSG_SIZE, 0, (struct sockaddr *)
             &server_addr, sizeof(server_addr)) < 0) {
    perror("send()");
    close(udp_sock);
    exit(-1);
  }
}

struct ServerToClient RecvMsg(void) {
  struct ServerToClient msg;

  if (recv(client_sock, &msg, SERVER_MSG_SIZE, 0) < 0) {
    perror("recv()");
    close(client_sock);
    close(tcp_listener_sock);
    exit(-1);
  }

  return msg;
}
