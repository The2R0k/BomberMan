#include "network.h"
#include "../include/position.h"

int tcp_sock, udp_sock;
int port;
char address[INET_ADDRSTRLEN];
char sendBuf[MESGSIZE];
char recvBuf[MESGSIZE];
struct sockaddr_in server_addr;

void initNetwork() {
  bzero(&server_addr, sizeof(server_addr));

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

void sendMsg(struct Position msg) {
  memcpy(sendBuf, &msg, MESGSIZE);

  if (sendto(udp_sock, sendBuf, MESGSIZE, 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("send()");
    close(udp_sock);
    exit(-1);
  }
}

void recvMsg() {
  if (recv(tcp_sock, recvBuf, MESGSIZE, 0) < 0) {
    perror("recv()");
    close(tcp_sock);
    exit(-1);
  }
}
