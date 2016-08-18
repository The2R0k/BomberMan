#include "header.h"

struct ConnectInfo {
  int id;
  int status;
  int sock_tcp;
  struct sockaddr_in from;
};

static uint8_t connects_count = DEFAULT;
static int sock_udp;
static struct ActionTable action_table;

static void SendLog(const char *message) {
  printf("New log message: %s\n", message);
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

  flags = fcntl(sock, F_GETFL, DEFAULT);
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

static void ActionTableClear() {
  int i;

  for(i = 0; i < MAX_PLAYERS; ++i) {
    action_table.player_info[i].suicide = DEFAULT;
    action_table.player_info[i].bomb = DEFAULT;
    action_table.player_info[i].move_pos.x = DEFAULT;
    action_table.player_info[i].move_pos.y = DEFAULT;
  }
}

static int8_t CreateSockets(struct ConnectInfo player_info[]) {
  int i;
  
  for (i = DEFAULT; i < TCP_CONNECTS; ++i) {
    player_info[i].id = i;
    player_info[i].status = OFFLINE;
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
  return 1;
}


static void HandleClientAction(uint8_t player_num, enum Doing action) {
  action_table.player_info[player_num].suicide = FALSE;
  switch (action) {
    case NOTHING:
      break;
    case PLANT_BOMB:
      action_table.player_info[player_num].bomb = TRUE;
      break;
    case MOVE_LEFT:
      action_table.player_info[player_num].move_pos.x = -1;
      action_table.player_info[player_num].move_pos.y = 0;
      break;
    case MOVE_RIGHT:
      action_table.player_info[player_num].move_pos.x = 1;
      action_table.player_info[player_num].move_pos.y = 0;
      break;
    case MOVE_TOP:
      action_table.player_info[player_num].move_pos.x = 0;
      action_table.player_info[player_num].move_pos.y = -1;
      break;
    case MOVE_DOWN:
      action_table.player_info[player_num].move_pos.x = 0;
      action_table.player_info[player_num].move_pos.y = 1;
      break;
    default:
      SendLog("unknown client action");
      break;
  }
}

static int8_t RunServer() {
  int i, bv;
  int back_value_array[MAX_PLAYERS];
  int new_player_flaq = FALSE;
  int connect_flaq = FALSE;

  struct ClientToServer *buffer;
  struct ServerToClient *msg;
  struct Field *game_field;
  struct StatsTable *stats_table;
  uint8_t player_id;

  struct sockaddr_in server;
  struct ConnectInfo player_info[TCP_CONNECTS];
  struct Field field;
  socklen_t server_len, from_len;
  
  struct timeval  elapsed,
                  time_from,
                  time_from_loop;
  double dt = 0;

  gettimeofday(&time_from, NULL);
  gettimeofday(&time_from_loop, NULL);

  /* Creating sockets. */
  if (!CreateSockets(player_info)) {
    return 0;
  }
  printf("udp socket is %d\n", sock_udp);

  /* UDP socket bind part. */
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

  /* Initialization. */
  i = DEFAULT;
  SetUp(&game_field, &stats_table);
  buffer = (struct ClientToServer*) malloc(sizeof(struct ClientToServer));
  msg   = (struct ServerToClient*) malloc(sizeof(struct ServerToClient));

  /* Program main cycle. */
  gettimeofday(&time_from, NULL);
  while (INFINITU_CYCLE) {    
    gettimeofday(&elapsed, NULL);
    dt = (elapsed.tv_sec - time_from_loop.tv_sec) * 1000;
    dt += (elapsed.tv_usec - time_from_loop.tv_usec) / 1000;
    if (dt < 16) {
      usleep(1000 * 16);
      continue;
    }
    gettimeofday(&time_from_loop, NULL); 
    from_len = sizeof(struct sockaddr_in);
    bv = recvfrom(sock_udp, buffer, sizeof(struct ClientToServer), 0,
            (struct sockaddr *) &player_info[DEFAULT].from, &from_len);
    if (bv > 0) {
      SendLog("got new message");
      printf("Bytes read: %hu\n", bv);
      printf("Id:%d, action:%d\n", buffer->id, buffer->doing);
      if (buffer->id != 0) {
        if (buffer->id > MAX_PLAYERS) {
           SendLog("unknown message sender");
        } else {
          HandleClientAction(buffer->id - 1, buffer->doing);
        }
      } else {
        bv = DEFAULT;
        from_len = sizeof(struct sockaddr_in);
        for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
          player_id = i + 1;
          if (player_info[player_id].status == ONLINE) {
            /* WTF is going here? */
            /* TODO: change this shit. */
            /* BEGIN shit. */
            if (player_info[player_id].from.sin_addr.s_addr == 
                player_info[DEFAULT].from.sin_addr.s_addr) {
              close(player_info[player_id].sock_tcp);
              bv = MakeSocket(MAKE_TCP_SOCK);
              if (bv > 0) {
                player_info[player_id].sock_tcp = bv;
                SendLog("player reconnected");
                new_player_flaq = player_id;
                break;
              } else {
                SendLog("Can't reopen sock");
                new_player_flaq = DEFAULT;
              }
            }
            /* END shit. */
          } else if (player_info[player_id].status == OFFLINE) {
            NewPlayer();
            new_player_flaq = player_id;
            player_info[player_id].status = ONLINE;
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
            player_info[new_player_flaq].status = ONLINE;

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
        }
      }
    }

    if (connects_count >= 1) {
/*          if(TimeCheck()>ONE_TICK_TIME){
      if(Tick()>=TICK_TO_START){
      TimeStart();                  */
      gettimeofday(&elapsed, NULL);
      dt = (elapsed.tv_sec - time_from.tv_sec) * 1000;
      dt += (elapsed.tv_usec - time_from.tv_usec) / 1000;  
      if (dt > 500) {
        gettimeofday(&time_from, NULL);
        Update(&action_table);
  /*    TimeStart();
  */    for (i = DEFAULT; i < connects_count; ++i) {
          msg->id = i + 1;
          printf("ID = %d\n", msg->id);
          memmove(&msg->field, game_field, sizeof(field));
          msg->stats.score  = stats_table->player_stats[i].score;
          msg->stats.length = stats_table->player_stats[i].length;
          msg->stats.death  = stats_table->player_stats[i].death;
          msg->stats.bomb   = stats_table->player_stats[i].bomb;
          back_value_array[i]=send(player_info[i + 1].sock_tcp, msg,
                sizeof(struct ServerToClient), 0);
          
          SendLog("sending to client");
          printf("back value array %d = %d", i, back_value_array[i]);
        }
        ActionTableClear();
      }
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
    i++;
  }

  free(buffer);
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
