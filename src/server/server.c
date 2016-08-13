#ifndef _SERVER_C_
#define _SERVER_C_

#include "header.h"

#define EPOLL_QUEUE_LEN 1
#define PORT_UDP 4444
#define PORT_TCP_S 4445 
#define PORT_TCP_CLIENT 9999
#define SERVER_IP "192.168.2.30"
#define TCP_CONNECTS 5
#define MAX_PLAYERS 4
#define INFINITU_CYCLE 1
#define DEFAULT 0
#define CONNECT_TRY_MAX 5

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

static uint8_t FullParty()
{
    if(connects_count < MAX_PLAYERS) return FALSE;
    return TRUE;
}

static int get_id()
{
    return(player_count++);
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

static int RunServer()
{
    int i,bv;
    int counter;
    int sock_udp;
    int new_player_flaq = FALSE;

    struct ClientToServer *bufer;
    struct ClientToServer *msg;
    struct Field *game_field;
//    struct StatsTable *stats_table;
//    struct ActionTable *action_table;

    struct sockaddr_in server;
    struct ConnectInfo playerInfo[TCP_CONNECTS];
    struct Field field;
    struct epoll_event ev, *events;
    socklen_t server_len, from_len;
    
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

    
    i=0;


    /*initialization here*/
    /*  
     *  SetUp(&game_field,&stats_table);
     *  
     */ 
    /*initialization end*/
    

    bufer = (struct ClientToServer*) malloc (sizeof(struct ClientToServer));
    msg = (struct ClientToServer*) malloc (sizeof(struct ClientToServer));

    while(INFINITU_CYCLE)
    {
    
        from_len = sizeof(struct sockaddr_in);
        bv = recvfrom(sock_udp,bufer,sizeof(struct ClientToServer),FALSE,
                (struct sockaddr *)&(playerInfo[DEFAULT].from),&from_len);

        if(bv>FALSE)
        {
          SendLog("get message");
          printf("%d:%d:%d:%d:%d;",bufer->id,bufer->bomb.x,bufer->bomb.y,bufer->move.x,bufer->move.y);
          printf("\n");
            
            switch(bufer->id)
            {
            
                case 0:
                        new_player_flaq++;
                        break;
/*                case 1:
                case 2:
                case 3:
                case 4:
                        action_table->player_info[bufer->id -1].suicide = 0;
                        if((bufer->bomb.x !=0)&(bufer->bomb.y !=0))
                        {
                            action_table->player_info[bufer->id -1].bomb_pos.x = bufer->bomb.x;
                            action_table->player_info[bufer->id -1].bomb_pos.y = bufer->bomb.y;
                        }
                        if((bufer->move.x !=0)&(bufer->move.y !=0))
                        {
                            action_table->player_info[bufer->id -1].move_pos.x = bufer->move.x;
                            action_table->player_info[bufer->id -1].move_pos.y = bufer->move.y;
                        }
                        break;
  */              default :
                        SendLog("unknown message");
                        break;
            }

            if(new_player_flaq)
            {
               
            player_count = DEFAULT;
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
                    bv = connect(playerInfo[new_player_flaq].sock_tcp,
                        (struct sockaddr*)&(playerInfo[new_player_flaq].from),
                        from_len);
                    perror("connecting::");
                    if(bv == DEFAULT)
                    {
                        SendLog("Connected");
                        playerInfo[i+1].status = TRUE;
/*  testing 
 *  must have ServerToClient structure 
 *  and another data 
 */

                        msg->id = new_player_flaq;
                        msg->bomb.x = DEFAULT;
                        msg->bomb.y = DEFAULT;
                        msg->move.x = DEFAULT;
                        msg->move.y = DEFAULT;
                        bv = send(playerInfo[new_player_flaq].sock_tcp,msg,
                                    sizeof(struct ClientToServer),DEFAULT);
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
                    sleep(1);                    
                }
            }
/*           {
 *               from_len = sizeof(struct sockaddr_in);
 *               sleep(1);
 *               printf("i'm get %d (%d) port",playerInfo[1].from.sin_port,ntohs(playerInfo[1].from.sin_port));
 *               playerInfo[1].from.sin_port = htons(9999);
 *               printf("port 9999 is %d \n",htons(9999));
 *               bv = connect(playerInfo[1].sock_tcp,(struct sockaddr*)&(playerInfo[1].from),from_len);
 *               perror("connect is::");
 *               if(bv < FALSE)
 *               {
 *                   printf("incorrect connect, try agan\n");
 *                   i++;
 *               }
 *               else
 *               {
 *                   printf("connected\n");
 *                   break;
 *               }
 *           }
 */   

    /* testing block */
            msg->id = 4;
            msg->bomb.x = 10;
            msg->bomb.y = 5;
            msg->move.x = 9;
            msg->move.y = 5;
    /* end testing block */            


            bv = send(playerInfo[1].sock_tcp, msg, sizeof(struct ClientToServer),0);
            
            if(bv <= FALSE)
            {
                printf("incorrect sending");
            }
            
            printf("sending\n");
        }

        printf("i'm work:%d\n",i);
        sleep(1);
        i++;

/*        return DEFAULT;*/
    }
}

int main()
{
    RunServer();
    return DEFAULT;
}

#endif
