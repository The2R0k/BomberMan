#include "gui.h"

/* Func getline needs. */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "network.h"
#include "../include/bgraphics.h"

#define EXIT_KEY 1

/*============================*/
/*                            */
/* Helpfull functions.        */
/*                            */
static int8_t IsAddressValid(char *ip_address);

static void PrintGreetings(void);

static void PrintMenu(void);

static void StartNewGame(void);

static void StartGameLoop(void);

static void TryConnect(void);

static int8_t HandleKeyPress(const Uint8 *keyboard_state);
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

int8_t HandleKeyPress(const Uint8 *keyboard_state) {
  enum Doing action = NOTHING;
  
  if (keyboard_state[SDL_SCANCODE_ESCAPE]) {
    return EXIT_KEY;
  }
  
  if (keyboard_state[SDL_SCANCODE_SPACE] ||
      keyboard_state[SDL_SCANCODE_RETURN]) { 
    action = PLANT_BOMB;
  } else if (keyboard_state[SDL_SCANCODE_UP]) {
    action = MOVE_TOP;
  } else  if (keyboard_state[SDL_SCANCODE_DOWN]) {
    action = MOVE_DOWN;
  } else if (keyboard_state[SDL_SCANCODE_RIGHT]) {
    action = MOVE_RIGHT;
  } else if (keyboard_state[SDL_SCANCODE_LEFT]) {
    action = MOVE_LEFT;
  }
  if (!HandleAction(action))
    return -1;

  return 0;
}

void StartGameLoop(void) {
  struct ServerToClient *msg;
  /* Possible states of game cycle's threads 
   * STABLE = 0, NEED_REFRESH = 1, NEED_EXIT = 2 */
  enum ThreadState *thread_state; 
  key_t shm_key;
  int shm_id;
  void *shm_mem;

  shm_key = ftok("tempfile", 1);
  if (shm_key == -1) {
    perror("ftok() error");
    exit(1);
  }
  shm_id = shmget(shm_key, sizeof(struct ServerToClient) +
           sizeof(enum ThreadState), IPC_CREAT | 0666);
  if (shm_id == -1) {
    perror("shmget() error");
    exit(1);
  }

  if ((shm_mem = shmat(shm_id, NULL, 0)) == (void *) -1) {
    perror("shmat() error");
    exit(1);
  }
  thread_state = ((uint8_t *) shm_mem) + sizeof(struct ServerToClient);
  *thread_state = STABLE;
  /* Cleaning up the pointers */
  thread_state = NULL;
  msg = NULL;
  shmdt(shm_mem);

  if (fork() == 0) {
    /* Child process. */
    /* SDL_Event datatype. Needs to follow "keypressed" events */
    SDL_Event event;
    /* To check current keyboard state - array of states for all the buttons */
    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
    /* This struct contains SDL graphic sources */
    struct GraphSources *gs = malloc(sizeof(struct GraphSources));
    int8_t key_handled;
    
    shm_mem = shmat(shm_id, NULL, 0);
    msg = shm_mem;
    thread_state = ((uint8_t *) shm_mem) + sizeof(struct ServerToClient);

    /* Waiting for accepting the data */
    while (*thread_state != NEED_REFRESH)
      SDL_Delay(5);
    /* Initializing graphics. This function 
     * (like a RefreshState and CleanGraph) works with pointer to 
     * struct GraphSiurces. */
    GraphicsInit(gs);
    while (*thread_state != NEED_EXIT) {
      /* Polling the events. */
      SDL_PollEvent(&event);
      /* Refreshing events array in memory. */ 
      SDL_PumpEvents(); 
      /* making delay = 18 ms: 
       * that's the best choice to follow keyboard events 
       * and do not load a processor. */
      SDL_Delay(18);

      /* Keyboard events processing */
      key_handled = HandleKeyPress(keyboard_state);
      if (key_handled == EXIT_KEY ||
          key_handled == -1) {
        *thread_state = NEED_EXIT;
        break;
      }
      /* If data from server is accepted... */ 
      if (*thread_state == NEED_REFRESH) {
        RefreshState(&msg->field, &msg->stats, gs);
        *thread_state = STABLE;
      }
    }
    /* Cleaning up shared and graphic resourses from current process. */
    CleanGraph(gs);
    free(gs);
    shmdt(shm_mem);
    exit(0);
  } else {
    /* Parent process. */
    /* Buffer to hold input struct's values */
    struct ServerToClient *buffer;
    uint8_t msg_received = 1;

    shm_mem = shmat(shm_id, NULL, 0);
    msg = shm_mem;

    /* Initializating pointer to enum ThreadState memory area */
    thread_state = ((uint8_t *) shm_mem) + sizeof(struct ServerToClient);
    /* While data is accepting successful and nobody pressed ESC */
    while (msg_received && (*thread_state != NEED_EXIT)) {
      /* Accepting the data from server */
      msg_received = RecvMsg(&buffer);
      if (!msg_received) {
        printf("Error: %d\n", msg_received);
        *thread_state = NEED_EXIT;
      } else {
        if (*thread_state == NEED_EXIT) {
          free(buffer);
          break;
        }
        /* Denotes that data is accepted and
         * we need to refresh the render image. */
        *thread_state = NEED_REFRESH;
        
        /* Translating buffer to shared memory. */
        memmove(msg, buffer, sizeof(struct ServerToClient));
      }
      free(buffer);
      buffer = NULL;
    }
    /* Cleaning up the data and closing application */
    wait(NULL);
    shmdt(shm_mem);
    msg = NULL;
    thread_state = NULL;
    shmctl(shm_id, IPC_RMID, 0);
  }
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
    ip_address[strlen(ip_address) - 1] = '\0';

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
    StartGameLoop();
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
    if (strlen(line) > 2) {
      c = '3';
    } else {
      c = line[0];
    }
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
