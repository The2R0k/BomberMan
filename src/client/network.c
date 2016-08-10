#include "network.h"

int tcp_sock, udp_sock;
int port;
char address[INET_ADDRSTRLEN];
const int on = 1;

void initNetwork() {
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(address);
  server_addr.sin_port = htons(port);

  /*Create tcp socket*/
  if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket()");
    exit(-1);
  }

  if (setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on)) < 0) {
    perror("setsockopt()");
    close(tcp_sock);
    exit(-1);
  }

  if (bind(tcp_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("bind()");
    close(tcp_sock);
    exit(-1);
  }

  if (listen(tcp_sock, 4) < 0) {
    perror("listen()");
    close(tcp_sock);
    exit(-1);
  }

  /*Create udp socket*/
  if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket()");
    exit(-1);
  }

  if (bind(udp_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("bind()");
    close(udp_sock);
    exit(-1);
  }

  /*connect tcp socket*/
  if (connect(tcp_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("connect()");
    exit(-1);
  }
}

void shutdownNetwork() {
  close(tcp_sock);
  close(udp_sock);
}
