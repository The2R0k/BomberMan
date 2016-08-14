#ifndef _SERVER_C_
#define _SERVER_C_

#include "header.h"

#define EPOLL_QUEUE_LEN 1
#define PORT_UDP 4444
#define PORT_TCP_S 4445 
#define PORT_TCP_CLIENT 9999
#define SERVER_IP "25.64.18.2"
#define TCP_CONNECTS 5
#define MAX_PLAYERS 4
#define INFINITU_CYCLE 1
#define DEFAULT 0        /* default - default value (always must been zero   */
#define CONNECT_TRY_MAX 15 
#define MIN_PLAYERS 2    /*How much player must been connected before Update */
#define ONE_TIK_TIME 500 /* one tik time it's time between Update(1) in mSec */

#define TRUE 1
#define FALSE 0


struct ConnectInfo{
    int id;
    int status;
    int sock_tcp;
    struct sockaddr_in from;
};

static uint8_t player_count = DEFAULT;
static uint8_t connects_count = DEFAULT;

static void SendLog(const char *message)
{

    printf("new log message::\n    %s\n",message);

}

static void NewPlayer()
{
    connects_count++;
}

static void SuidPlayer()
{
    connects_count--;
}

static int MakeSocketNonBlocking (int sock)
{
    int flags, s;
    int opt_flag = 1;
    flags = fcntl (sock, F_GETFL, DEFAULT);
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }
    
    flags |= O_NONBLOCK;
    s = fcntl (sock, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }

    

    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR, &opt_flag,sizeof(int));

    return DEFAULT;
}

static struct tms tmsBegin,tmsEnd;

void TimeStart(){
  times(&tmsBegin); 
}

static long TimeCheck(){

  times(&tmsEnd);

  return (((tmsEnd.tms_utime-tmsBegin.tms_utime)+
          (tmsEnd.tms_stime-tmsBegin.tms_stime))*1000/CLOCKS_PER_SEC);
}

static void ActionTableClear(struct ActionTable *action_table){
  int i=DEFAULT;
  for(;i<MAX_PLAYERS;++i){
    action_table->player_info[i].suicide = DEFAULT;
    action_table->player_info[i].plant_bomb = DEFAULT;
    action_table->player_info[i].move.x = DEFAULT;
    action_table->player_info[i].move.y = DEFAULT;
  }  
}

static int RunServer()
{
    int i,bv;
    int back_value_array[MAX_PLAYERS];
    int sock_udp;
    int new_player_flaq = FALSE;

    struct ClientToServer *bufer;
    struct ServerToClient *msg;
    struct Field *game_field;
    struct StatsTable *stats_table;
    struct ActionTable action_table;

    struct sockaddr_in server;
    struct ConnectInfo playerInfo[TCP_CONNECTS];
    struct Field field;
    socklen_t server_len, from_len;
    
    /* make sockets */

    for(i=DEFAULT;i<TCP_CONNECTS;++i)
    {
        playerInfo[i].id = i;
        playerInfo[i].status = FALSE;
        playerInfo[i].sock_tcp = socket(AF_INET,SOCK_STREAM,DEFAULT);
        if(playerInfo[i].sock_tcp <= FALSE)
        {
            printf("%d::",i);
            perror("can't make socket::");
            return 1;
        }
        printf("new socket is:%d \n",playerInfo[i].sock_tcp);

    }
    sock_udp = socket(AF_INET,SOCK_DGRAM,DEFAULT);
   
    if(sock_udp <= FALSE)
    {
        printf("can't make socket\n");
        return 1;
    }
    
    /* UDP socket bind part */

    MakeSocketNonBlocking(sock_udp);

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_UDP);
    
    server_len = sizeof(struct sockaddr_in);    

    bv = bind(sock_udp, (struct sockaddr*)&server,server_len);
    if(bv < FALSE)
    {
        perror("can't bind udp:");
        return 1;
    }
    
    /*TCP BIND PART */

    for(i=DEFAULT;i<TCP_CONNECTS;++i)
    {
        MakeSocketNonBlocking(playerInfo[i].sock_tcp);
        
        server.sin_port = htons(PORT_TCP_S + i);
        server_len = sizeof(struct sockaddr_in);
        bv = bind(playerInfo[i].sock_tcp,(struct sockaddr*)&server,server_len);
        printf("now bv is %d\n",bv);
        if(bv < DEFAULT)
        {
            perror("can't bind someone tcp::");
            return 1;
        }
        
    }

    /*initialization here*/
    
    i=DEFAULT;
    SetUp(&game_field,&stats_table);
    bufer = (struct ClientToServer*) malloc (sizeof(struct ClientToServer));
    msg = (struct ServerToClient*) malloc (sizeof(struct ServerToClient));
    TimeStart();
    /*initialization end*/

    /* program main cycle (always work) */
    while(INFINITU_CYCLE)
    {    
        from_len = sizeof(struct sockaddr_in);
        bv = recvfrom(sock_udp,bufer,sizeof(struct ClientToServer),FALSE,
                (struct sockaddr *)&(playerInfo[DEFAULT].from),&from_len);
        if(bv>FALSE)
        {
          SendLog("get message");
          printf("%d:%d;",bufer->id,bufer->doing);
          printf("\n");
          if(bufer->id)
          {
            if(bufer->id>MAX_PLAYERS)
            {
               SendLog("Unknown message sender");
            }
            else
            {
                action_table.player_info[bufer->id].suicide = FALSE;
                switch(bufer->doing)
                {
                    case NOTHING:
                        break;
                    case PLANT_BOMB:
                        action_table.player_info[bufer->id].plant_bomb = TRUE;
                        break;
                    case MOVE_LEFT:
                        action_table.player_info[bufer->id].move.x = -1;
                        action_table.player_info[bufer->id].move.y = 0;
                        break;
                    case MOVE_RIGHT:
                        action_table.player_info[bufer->id].move.x = 1;
                        action_table.player_info[bufer->id].move.y = 0;
                        break;
                    case MOVE_TOP:
                        action_table.player_info[bufer->id].move.x = 0;
                        action_table.player_info[bufer->id].move.y = -1;
                        break;
                    case MOVE_DOWN:
                        action_table.player_info[bufer->id].move.x = 0;
                        action_table.player_info[bufer->id].move.y = 1;
                        break;
                    default:
                        SendLog("i don't know what client won't");
                        break;
                }
            }
          }
          else
          {
            bv = DEFAULT;
            from_len = sizeof(struct sockaddr_in);
            for(i=DEFAULT;i<MAX_PLAYER_AMOUNT;++i)
            {
                if(!playerInfo[i+1].status)
                {
                    new_player_flaq = i+1;
                    SendLog("we are have a new player");
                    break;
                }
                new_player_flaq = FALSE;
            }

            playerInfo[new_player_flaq].from.sin_port = 
                htons(PORT_TCP_CLIENT);
            playerInfo[new_player_flaq].from.sin_family = AF_INET;
            playerInfo[new_player_flaq].from.sin_addr.s_addr = 
                playerInfo[DEFAULT].from.sin_addr.s_addr;

            printf("new_player_flaq is :: %d",new_player_flaq);
            for(i=DEFAULT;i<CONNECT_TRY_MAX;++i)
            {
                SendLog("try connecting to client");
                bv  = connect(playerInfo[new_player_flaq].sock_tcp,                 /*<<<<<<<<< can drop here */
                    (struct sockaddr*)&(playerInfo[new_player_flaq].from),
                    from_len);
                perror("connecting::");
                if(bv == DEFAULT)
                {
                    SendLog("Connected");
                    playerInfo[i+1].status = TRUE;
                    NewPlayer();

                    msg->id = new_player_flaq;
                    memmove(&msg->field,&field,sizeof(field));
                    msg->stats.score = DEFAULT;
                    msg->stats.length = DEFAULT;
                    msg->stats.death = DEFAULT;
                    msg->stats.bomb = DEFAULT;

                    bv = send(playerInfo[new_player_flaq].sock_tcp,msg,
                       sizeof(struct ServerToClient),DEFAULT);
                    printf("now BV is :: %d \n",bv);
                    if(bv)
                    {
                        SendLog("sending complite sucefull");
                        break;
                    }
                }
                if(i==CONNECT_TRY_MAX -1)
                {
                    SendLog("Connecting filed: unknown");
                }
            }
            if(player_count >= MIN_PLAYERS){
              if(TimeCheck()>ONE_TIK_TIME){
/*                TimeStart();                  */
                Update(&action_table);
                TimeStart();
                for(i=DEFAULT;i<MAX_PLAYERS;++i){
                  msg->id = i+1;
                  memmove(&msg->field,&field,sizeof(field));
                  msg->stats.score = stats_table->player_stats[i].score;
                  msg->stats.length = stats_table->player_stats[i].length;
                  msg->stats.death = stats_table->player_stats[i].death;
                  msg->stats.bomb = stats_table->player_stats[i].bomb;
                  back_value_array[i]=send(playerInfo[i+1].sock_tcp,msg,
                        sizeof(struct ServerToClient),DEFAULT);
                }
                for(i=DEFAULT;i<MAX_PLAYERS;++i){
                  if(back_value_array[i] < FALSE){
                    action_table.player_info[i].suicide = TRUE;
                    playerInfo[i+1].status = FALSE;
                    SuidPlayer();
                  }
                }
                ActionTableClear(&action_table);
              }
            }
          }
        }
      printf("i'm work:%d\n",i);
      i++;
    }
}

int main()
{
    RunServer();
    return DEFAULT;
}

#endif
