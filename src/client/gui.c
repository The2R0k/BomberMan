#include "gui.h"

/* Func getline needs. */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>

#include "network.h"

/*============================*/
/*                            */
/* Helpfull functions.        */
/*                            */
static int8_t IsAddressValid(char *ip_address);

static void PrintGreetings(void);

static void PrintMenu(void);

static void StartNewGame(void);

static void StartGameCircle(void);

static void TryConnect(void);

/*============================*/
/*                            */
/* Definitions.               */
/*                            */
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
    execlp("./server", "server", NULL);
  } else { 
    Connect("127.0.0.1"); /* TEST: */
  }
}

/* TODO: remove after debugging. */
static void PrintMap(struct Field *field) {
  int i, j;

  for (i = 0; i < FIELD_SIZE; ++i) {
    for (j = 0; j < FIELD_SIZE; ++j) {
      printf("%hu", field->location[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void StartGameCircle(void) {
  struct ServerToClient *msg = NULL;
  int i = 0;
  int8_t success = 1;

  while (success) {
    if (!(success = RecvMsg(&msg))) {
      printf("Error: %d\n", success);
    } else {
      printf("Recv map\n");
      PrintMap(&msg->field);
      free(msg);
      msg = NULL;
      printf("Iteration %d\n", ++i);
    }
  } 
  /* TODO: run graphics.
   * It should be such as:
   *
   * struct GameInfo game_info;
   *
   * InitializeGraphics();
   * while (1) {
   *   game_info = GetNewState();
   *   GameUpdate(&game_info);
   *   
   *   Note: we don't need sleep, 'cause GetNewState() is blocking operation,
   *   which calls RecvMsg from server inside.
   *   TODO: think how end the game.
   * }
   */
}

void TryConnect(void) {
  char *ip_address = NULL;
  size_t count = 0;
  int8_t need_exit = 0;
  size_t i;
  int8_t is_error = 0;
  
  while (!need_exit && !is_error) {
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
      if (Connect(ip_address)) {
        need_exit = 1;
      } else {
        is_error = 1;
      } 
    }
    else {
      printf("Incorrect ip\n");
      printf("============\n");
    }

    free(ip_address);
    ip_address = NULL;
    count = 0;
  }
  if (!is_error) {
    StartGameCircle();
  } else {
    printf("Error: can't connect to server.\n");
  }
}

void Run(void) {
  char c;
  int8_t need_exit = 0;
  char *line = NULL;
  size_t count = 0;
 
  /* TODO: add after debugging. */
  /* close(STDERR_FILENO); */
  PrintGreetings();
  while (!need_exit) {
    PrintMenu();
    getline(&line, &count, stdin);
    c = line[0];
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
    free(line);
    line = NULL;
    count = 0;
  }
}
