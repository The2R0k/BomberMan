#include "header.h"

#include <unistd.h>

#define EPOLL_QUEUE_LEN 1
#define PORT_UDP 4444
#define PORT_TCP_S 4445 
#define PORT_TCP_CLIENT 9999
#define SERVER_IP "25.64.18.2"

#define TCP_CONNECTS 5
#define MAX_PLAYERS 4
#define MIN_PLAYERS 2  /* How many player should be connected before Update. */

#define MAKE_TCP_SOCK 1
#define MAKE_UDP_SOCK 2

#define TICK_TO_START 6
#define INFINITU_CYCLE 1
#define DEFAULT 0        /* Default value (always must been zero). */
#define CONNECT_TRY_MAX 5 
#define ONE_TICK_TIME 500 /* Time between one Update in mSec. */

#define TRUE 1
#define FALSE 0

struct ConnectInfo {
  int id;
  int status;
  int sock_tcp;
  struct sockaddr_in from;
};

static uint8_t connects_count = DEFAULT;
static uint8_t tick_counter = DEFAULT;
static struct tms tmsBegin, tmsEnd;
static int sock_udp;

static int Tick(void) {
  return tick_counter++;
}

static void SendLog(const char *message) {
  printf("new log message::\n    %s\n",message);
}

static void NewPlayer(void) {
  connects_count++;
}

static void SuidPlayer(void) {
  connects_count--;
}

static int MakeSocketNonBlocking(int sock) {
  int flags, s;
  int opt_flag = 1;

  flags = fcntl (sock, F_GETFL, DEFAULT);
  if (flags == -1) {
    perror ("fcntl");
    return -1;
  }
  
  flags |= O_NONBLOCK;
  s = fcntl (sock, F_SETFL, flags);
  if (s == -1) {
    perror ("fcntl");
    return -1;
  }

  setsockopt(sock,SOL_SOCKET,SO_REUSEADDR, &opt_flag,sizeof(int));

  return DEFAULT;
}

static int MakeSocket(int flaq) {
  int bv;
  
  if (flaq == MAKE_TCP_SOCK) {
    bv = socket(AF_INET, SOCK_STREAM, DEFAULT);
  } else {
    bv = socket(AF_INET, SOCK_DGRAM, DEFAULT);
  }
  if (bv < 0) {
    perror("Socket error");
  }

  return bv;
}


void TimeStart(void) {
  times(&tmsBegin); 
}

static long TimeCheck(void) {
  times(&tmsEnd);
  Tick();

  return (((tmsEnd.tms_utime - tmsBegin.tms_utime) +
          (tmsEnd.tms_stime - tmsBegin.tms_stime)) * 1000);
}

static void ActionTableClear(struct ActionTable *action_table) {
  int i;

  for(i = 0; i < MAX_PLAYERS; ++i) {
    action_table->player_info[i].suicide = DEFAULT;
    action_table->player_info[i].bomb = DEFAULT;
    action_table->player_info[i].move_pos.x = DEFAULT;
    action_table->player_info[i].move_pos.y = DEFAULT;
  }
}

static int8_t CreateSockets(struct ConnectInfo player_info[]) {
  int i;
  
  for (i = DEFAULT; i < TCP_CONNECTS; ++i) {
    player_info[i].id = i;
    player_info[i].status = FALSE;
    player_info[i].sock_tcp = MakeSocket(MAKE_TCP_SOCK);
    if(player_info[i].sock_tcp < 0) {
      printf("%d ", i);
      perror("CreateSockets() error");
      return 0;
    }
    printf("new socket is:%d \n", player_info[i].sock_tcp);

  }
  sock_udp = MakeSocket(MAKE_UDP_SOCK);;
  if (sock_udp < 0) {
    perror("CreateSockets() error");
    return 0;
  }
}

static int8_t RunServer() {
  int i,bv;
  int back_value_array[MAX_PLAYERS];
  int new_player_flaq = FALSE;
  int connect_flaq = FALSE;

  struct ClientToServer *bufer;
  struct ServerToClient *msg;
  struct Field *game_field;
  struct StatsTable *stats_table;
  struct ActionTable action_table;

  struct sockaddr_in server;
  struct ConnectInfo player_info[TCP_CONNECTS];
  struct Field field;
  socklen_t server_len, from_len;
  
  /* make sockets */
  if (!CreateSockets(player_info)) {
    return 0;
  }
  printf("udp socket is %d\n", sock_udp);

  /* UDP socket bind part */
  MakeSocketNonBlocking(sock_udp);

  server.sin_addr.s_addr = inet_addr(SERVER_IP);
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT_UDP); 
  server_len = sizeof(struct sockaddr_in);    

  bv = bind(sock_udp, (struct sockaddr*) &server, server_len);
  if (bv < 0) {
    perror("Bind udp error");
    return 0;
  }
  
  /* TCP bind part. */
  for (i = DEFAULT; i < TCP_CONNECTS; ++i) {
    MakeSocketNonBlocking(player_info[i].sock_tcp);
    
    server.sin_port = htons(PORT_TCP_S + i);
    server_len = sizeof(struct sockaddr_in);
    bv = bind(player_info[i].sock_tcp, (struct sockaddr*) &server, server_len);
    printf("now bv is %d\n",bv);
    if (bv < DEFAULT) {
        perror("can't bind someone tcp::");
        return 1;
    }  
  }

  /*initialization here*/
  
  i = DEFAULT;
  SetUp(&game_field, &stats_table);
  bufer = (struct ClientToServer*) malloc(sizeof(struct ClientToServer));
  msg   = (struct ServerToClient*) malloc(sizeof(struct ServerToClient));
  TimeStart();
  /*initialization end*/

  /* program main cycle (always work) */
  while (INFINITU_CYCLE) {    
    from_len = sizeof(struct sockaddr_in);
    bv = recvfrom(sock_udp, bufer, sizeof(struct ClientToServer), 0,
            (struct sockaddr *) &player_info[DEFAULT].from, &from_len);
    if (bv > 0) {
      SendLog("\n\nget message");
      printf("%d:%d;", bufer->id, bufer->doing);
      printf("\n");
      if(bufer->id) {
        if(bufer->id>MAX_PLAYERS) {
           SendLog("Unknown message sender");
        } else {
          action_table.player_info[bufer->id].suicide = FALSE;
          switch (bufer->doing) {
            case NOTHING:
              break;
            case PLANT_BOMB:
              action_table.player_info[bufer->id].bomb = TRUE;
              break;
            case MOVE_LEFT:
              action_table.player_info[bufer->id].move_pos.x = -1;
              action_table.player_info[bufer->id].move_pos.y = 0;
              break;
            case MOVE_RIGHT:
              action_table.player_info[bufer->id].move_pos.x = 1;
              action_table.player_info[bufer->id].move_pos.y = 0;
              break;
            case MOVE_TOP:
              action_table.player_info[bufer->id].move_pos.x = 0;
              action_table.player_info[bufer->id].move_pos.y = -1;
              break;
            case MOVE_DOWN:
              action_table.player_info[bufer->id].move_pos.x = 0;
              action_table.player_info[bufer->id].move_pos.y = 1;
              break;
            default:
              SendLog("i don't know what client want");
              break;
          }
        } else {

      } else {
        bv = DEFAULT;
        from_len = sizeof(struct sockaddr_in);
        for (i = DEFAULT; i < MAX_PLAYER_AMOUNT; ++i) {
          if(player_info[i + 1].status == TRUE){
            if(player_info[i + 1].from.sin_addr.s_addr == 
                player_info[DEFAULT].from.sin_addr.s_addr) {
              close(player_info[i+1].sock_tcp);
              bv = MakeSocket(MAKE_TCP_SOCK);
              if (bv > 0) {
                player_info[i+1].sock_tcp = bv;
                SendLog("player reconnected");
                new_player_flaq = i+1;
                break;
              } else {
                SendLog("Can't reopen sock");
                new_player_flaq = DEFAULT;
              }
            }
          }
          if (player_info[i + 1].status == FALSE) {
            NewPlayer();
            new_player_flaq = i + 1;
            player_info[i + 1].status = TRUE;
            printf("new_player_flaq is :: %d\n", new_player_flaq);
            SendLog("we are have a new player");
            break;
          }
          new_player_flaq = DEFAULT;
        }

        player_info[new_player_flaq].from.sin_port = htons(PORT_TCP_CLIENT);
        player_info[new_player_flaq].from.sin_family = AF_INET;
        player_info[new_player_flaq].from.sin_addr.s_addr = 
            player_info[DEFAULT].from.sin_addr.s_addr;

        for (i = DEFAULT; i < CONNECT_TRY_MAX; ++i) {
          SendLog("try connecting to client");
          bv  = connect(player_info[new_player_flaq].sock_tcp, 
              (struct sockaddr *) &(player_info[new_player_flaq].from),
              from_len);
          perror("connecting::");
          if (bv == DEFAULT) {
            connect_flaq = TRUE;
          }
          if (errno == EALREADY) {
            connect_flaq = TRUE;
          }
          if (errno == 106) {
            connect_flaq = TRUE;
          }
          
          if (connect_flaq) {
            SendLog("Connected");
            player_info[new_player_flaq].status = TRUE;

            msg->id = new_player_flaq;
            memmove(&(msg->field), game_field, sizeof(field));
            
            msg->stats.score = DEFAULT;
            msg->stats.length = DEFAULT;
            msg->stats.death = DEFAULT;
            msg->stats.bomb = DEFAULT;
            bv = send(player_info[new_player_flaq].sock_tcp, msg,
               sizeof(struct ServerToClient), MSG_DONTWAIT);
            perror("connect::");
            connect_flaq = FALSE;
            printf("sended bytes : %d\n", bv);
            if (bv > 0) {
              SendLog("sending complite sucefull");
              break;
            }
          }
          if (i == CONNECT_TRY_MAX - 1) {
            SendLog("Connecting filed: unknown");
          }
          sleep(1);
        }
      }
    }

    if (connects_count >= 1) {
/*          if(TimeCheck()>ONE_TICK_TIME){
      if(Tick()>=TICK_TO_START){
      TimeStart();                  */
      Update(&action_table);
/*    TimeStart();
*/    for (i = DEFAULT; i < connects_count; ++i) {
        msg->id = i + 1;
        printf("ID = %d\n", msg->id);
        memmove(&msg->field, &game_field, sizeof(field));
        msg->stats.score  = stats_table->player_stats[i].score;
        msg->stats.length = stats_table->player_stats[i].length;
        msg->stats.death  = stats_table->player_stats[i].death;
        msg->stats.bomb   = stats_table->player_stats[i].bomb;
        back_value_array[i]=send(player_info[i + 1].sock_tcp, msg,
              sizeof(struct ServerToClient), 0);
        
        SendLog("sending to client");
        printf("back value array %d = %d", i, back_value_array[i]);
      }
      ActionTableClear(&action_table);
/*              for(i=DEFAULT;i<max_players;++i){
        if(back_value_array[i] < FALSE){
          action_table.player_info[i].suicide = TRUE;
          SuidPlayer();
          SendLog("we are have a suid player");
        }
      }
*/    if(action_table.player_info[0].suicide == TRUE){
        break;
      }
    }
/*          }
      } */
    sleep(1);
    i++;
  }

  free(bufer);
  free(msg);
  for (i = DEFAULT; i < TCP_CONNECTS; ++i) {
    close(player_info[i].sock_tcp);
  }
  close(sock_udp);
  return DEFAULT;
}

int main()
{
  int bv;
  bv = RunServer();
  if (bv) {
    printf("Something went wrong!\n");
  }
  return DEFAULT;
}
