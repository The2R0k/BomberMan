#include "network.h"

/* Global variables */
int tcp_sock, udp_sock, client_sock;
char tmp;
char *address;
struct sockaddr_in client_addr, server_addr;
static int state;

void initUDP(char const *argv[]) {

  if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket()");
    exit(-1);
  }

  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  server_addr.sin_port = htons(atoi(argv[2]));
}

void initTCP() {
  struct ifreq ifr;
  const int on = 1;
  socklen_t server_len;

  if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket()");
    exit(-1);
  }

  /* get an IPv4 IP address */
  ifr.ifr_addr.sa_family = AF_INET;

  /* IP address attached to "enp2s0f0" */
  strncpy(ifr.ifr_name, "enp2s0f0", IFNAMSIZ - 1);
  ioctl(tcp_sock, SIOCGIFADDR, &ifr);
  address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
  // printf("%s\n", address);

  bzero(&client_addr, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = inet_addr(address);
  client_addr.sin_port = htons(PORT);

  if (setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on)) < 0) {
    perror("setsockopt()");
    close(tcp_sock);
    exit(-1);
  }

  if (bind(tcp_sock, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
    perror("bind()");
    close(tcp_sock);
    exit(-1);
  }

  if (listen(tcp_sock, 1) < 0) {
    perror("listen()");
    close(tcp_sock);
    exit(-1);
  }

  server_len = sizeof(server_addr);
  if((client_sock = accept(tcp_sock, (struct sockaddr*)&server_addr, &server_len)) < 0) {
    perror("accept()");
    exit(-1);
  }
}

void shutdownNetwork() {
  close(tcp_sock);
  close(udp_sock);
}

void registration(struct ClientToServer *msg) {
  int bytes = 0;

  msg->id = 0;
  msg->bomb.x = 0;
  msg->bomb.y = 0;
  msg->move.x = 0;
  msg->move.y = 0;

  if ((bytes = sendto(udp_sock, msg, CLIENT_MSG_SIZE, 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)) {
    printf("BYTES = %d\n", bytes);
    perror("send()");
    close(udp_sock);
    exit(-1);
  }
}

void sendMsg(struct ClientToServer *msg) {
  if (sendto(udp_sock, msg, CLIENT_MSG_SIZE, 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("send()");
    close(udp_sock);
    exit(-1);
  }
}

struct ServerToClient * recvMsg() {
  struct ServerToClient *recvMsg;

  if (recv(client_sock, &recvMsg, SERVER_MSG_SIZE, 0) < 0) {
    perror("recv()");
    close(client_sock);
    close(tcp_sock);
    exit(-1);
  }

  return recvMsg;
}

/*for debug*/
struct ClientToServer * recvMsg2() {
  struct ClientToServer *recvMsg;

  if (recv(tcp_sock, &recvMsg, SERVER_MSG_SIZE, 0) < 0) {
    perror("recv()");
    close(client_sock);
    close(tcp_sock);
    exit(-1);
  }

  return recvMsg;
}

void runClient(char const *argv[], struct ClientToServer *msg) {
  switch (state) {
    case CONNECT_NEW_PLAYER:
      initUDP(argv);
      initTCP();
      registration(msg);
      break;

    case PLAYER_DID_ACTION:
      sendMsg(msg);

      break;
  }
}
