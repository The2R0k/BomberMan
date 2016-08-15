#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/client_server.h"

int main(void) {
  int udp_sock;
  struct sockaddr_in client_address,
                     server_address;
  socklen_t len;
  struct ClientToServer msg;

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(4444);
  server_address.sin_addr.s_addr = inet_addr("25.59.28.123");

  udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  len = sizeof(struct sockaddr_in);
  bind(udp_sock, (struct sockaddr *) &server_address, len);

  if (recvfrom(udp_sock, &msg, sizeof(struct ClientToServer), 0,
           (struct sockaddr *) &server_address, &len) < 0) {
    perror("recv");
    exit(1);
  }
  printf("id: %d, do: %d\n", msg.id, msg.doing);
  close(udp_sock);

  return 0;
}
