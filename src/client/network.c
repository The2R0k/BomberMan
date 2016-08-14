#include "network.h"

#define SERVER_PORT 4444
#define CLIENT_PORT 9999

/* Global variables */
static int tcp_sock, udp_sock, client_sock;
static char tmp;
static char *address;
static struct sockaddr_in client_addr, server_addr;
static int state;

void InitUDP(char *server_ip) {
  if ((udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("socket() error");
    exit(-1);
  }

  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  server_addr.sin_port = htons(CLIENT_PORT);
}

void InitTCP() {
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

  if (setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on,
                 sizeof(on)) < 0) {
    perror("setsockopt() error");
    close(tcp_sock);
    exit(-1);
  }

  if (bind(tcp_sock, (struct sockaddr *) &client_addr,
           sizeof(client_addr)) < 0) {
    perror("bind() error");
    close(tcp_sock);
    exit(-1);
  }

  if (listen(tcp_sock, 1) < 0) {
    perror("listen() error");
    close(tcp_sock);
    exit(-1);
  }

  server_len = sizeof(server_addr);
  if((client_sock = accept(tcp_sock, (struct sockaddr*)&server_addr,
                           &server_len)) < 0) {
    perror("accept() error");
    exit(-1);
  }
}

void ShutdownNetwork() {
  close(tcp_sock);
  close(udp_sock);
}

void Registration(struct ClientToServer *msg) {
  msg->id = 0;
  msg->bomb.x = 0;
  msg->bomb.y = 0;
  msg->move.x = 0;
  msg->move.y = 0;

  if (sendto(udp_sock, msg, CLIENT_MSG_SIZE, 0, (struct sockaddr *)
                      &server_addr, sizeof(server_addr)) < 0) {
    perror("send()");
    close(udp_sock);
    exit(-1);
  }
}

void SendMsg(struct ClientToServer *msg) {
  if (sendto(udp_sock, msg, CLIENT_MSG_SIZE, 0, (struct sockaddr *)
             &server_addr, sizeof(server_addr)) < 0) {
    perror("send()");
    close(udp_sock);
    exit(-1);
  }
}

struct ServerToClient *RecvMsg() {
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
struct ClientToServer *RecvMsg2() {
  struct ClientToServer *recvMsg;

  if (recv(tcp_sock, &recvMsg, SERVER_MSG_SIZE, 0) < 0) {
    perror("recv()");
    close(client_sock);
    close(tcp_sock);
    exit(-1);
  }

  return recvMsg;
}

void RunClient(char const *argv[], struct ClientToServer *msg) {
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