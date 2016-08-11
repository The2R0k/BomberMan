#include "network.h"
#include "../include/action_table.h"

int tcp_sock, udp_sock;
int port;
char address[INET_ADDRSTRLEN];
char sendBuf[ACTIONSIZE];
char recvBuf[ACTIONSIZE];
struct sockaddr_in server_addr;

void initNetwork() {
  bzero(&server_addr, sizeof(server_addr));

  /*  take ip/port from config file
   *  format example: 127.0.0.1:4444
   */
  FILE *fd;
  if ((fd = fopen("config", "r")) < 0) {
    perror("fopen()");
    exit(-1);
  }

  fgets(address, sizeof(address), fd);

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(address);
  server_addr.sin_port = htons(port);

  /*Create tcp socket*/
  if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket()");
    exit(-1);
  }

  /*connect tcp socket*/
  if (connect(tcp_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("connect()");
    exit(-1);
  }

  /*Create udp socket*/
  if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket()");
    exit(-1);
  }
}

void shutdownNetwork() {
  close(tcp_sock);
  close(udp_sock);
}

void sendMsg(struct ActionCell *msg) {
  memcpy(sendBuf, &msg, ACTIONSIZE);
  msg->suicide = 0;

  if (sendto(udp_sock, sendBuf, ACTIONSIZE, 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("send()");
    close(udp_sock);
    exit(-1);
  }
}

void recvMsg() {
  if (recv(tcp_sock, recvBuf, ACTIONSIZE, 0) < 0) {
    perror("recv()");
    close(tcp_sock);
    exit(-1);
  }
}
