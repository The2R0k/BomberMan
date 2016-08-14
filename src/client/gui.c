#include "gui.h"

#include "network.h"

#include <stdio.h>

int8_t IsAddressValid(char *ip_address) {
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, ip_address, &(sa.sin_addr));
  return result == 1;
}

void PrintGreetings(void) {
  printf("Bomberman\n");
  printf("\n");
}

void PrintMenu(void) {
  printf("Menu:\n");
  printf("1 - Create new game\n");
  printf("2 - Connect\n");
  printf("0 - Exit\n");
}

void StartNewGame(void) {
  if (fork() == 0) {
    execlp("./server", NULL);
  } else { 
    Connect("127.0.0.1"); /* TEST: */
  }
}

int8_t Connect(char *ip_address) {  
  printf("Init udp\n");
  InitUDP(ip_address);
  printf("Registration\n");
  Registration();
  printf("Init tcp\n");
  InitTCP(ip_address);
  printf("Done\n");
  
  return 1;
}

void TryConnect(void) {
  char *ip_address = NULL;
  size_t count = 0;
  int8_t need_exit = 0;
  size_t i;
  
  while (!need_exit) {
    printf("Write server ip address\n");
    printf("q - quit\n");
    printf("ip: ");
    if (getline(&ip_address, &count, stdin) < 0) {
      perror("getline error()");
      printf("Incorrect input\n");
      
      free(ip_address);
      ip_address = NULL;
      count = 0;
      continue;
    }
    if (ip_address[0] == 'q' && ip_address[1] == '\n')
      exit(0);

    /* Removing break symbol. */
    for (i = 0; i < count; ++i) {
      if (ip_address[i] == '\n') {
        ip_address[i] = '\0';
        break;
      }
    }

    if (IsAddressValid(ip_address)) {
      if (Connect(ip_address))
        need_exit = 1;
    }
    else {
      printf("Incorrect ip\n");
      printf("============\n");
    }

    free(ip_address);
    ip_address = NULL;
    count = 0;
  } 
}

void ShowStartingMenu(void) {
  char c, b_sym;
  int8_t need_exit = 0;
 
  close(STDERR_FILENO); 
  PrintGreetings();
  while (!need_exit) {
    PrintMenu();
    c = getchar();
    b_sym = getchar(); /* Break symbol. */
    if ('0' <= c && c <= '2') {
      need_exit = 1;
    }
    switch (c) {
      case '1':
        StartNewGame();
        break;
      case '2':
        TryConnect();
        break;
      case '0':
        exit(0);
        break;
      default:
        printf("Incorrect input, try again\n");
        printf("==========================\n");
        break;
    }
  }
}
