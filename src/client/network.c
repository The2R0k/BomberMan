#include "network.h"

static int state;
int tcp_sock, udp_sock;
char tmp;
char sendBuf[MSGSIZE];
char recvBuf[MSGSIZE];
struct sockaddr_in server_addr;

void initNetwork(char const *argv[]) {
  bzero(&server_addr, sizeof(server_addr));

  /* take ip/port from argv
   * format: ./client 192.168.0.2 4444
   */
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  server_addr.sin_port = htons(atoi(argv[2]));

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

void sendMsg(struct MsgToServer *msg) {
  if (state == CONNECT_NEW_PLAYER) {
    msg->id = 0;
    msg->bomb_pos.x = 0;
    msg->bomb_pos.y = 0;
    msg->move_pos.x = 0;
    msg->move_pos.y = 0;

    memcpy(sendBuf, &msg, MSGSIZE);

    if (sendto(udp_sock, sendBuf, MSGSIZE, 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
      perror("send()");
      close(udp_sock);
      exit(-1);
    }
  } else if (state == PLAYER_DID_ACTION) {
    memcpy(sendBuf, &msg, MSGSIZE);

    if (sendto(udp_sock, sendBuf, MSGSIZE, 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
      perror("send()");
      close(udp_sock);
      exit(-1);
    }
  }
}

char recvMsg() {
  if (recv(tcp_sock, recvBuf, MSGSIZE, 0) < 0) {
    perror("recv()");
    close(tcp_sock);
    exit(-1);
  }

  return *recvBuf;
}
