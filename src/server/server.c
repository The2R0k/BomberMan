#ifndef _SERVER_C_
#define _SERVER_C_

#include "header.h"

#define EPOLL_QUEUE_LEN 1
#define PORT_UDP 4444
#define PORT_TCP_S 4445 
#define SERVER_IP "192.168.2.30"
#define TCP_CONNECTS 5
#define MAX_PLAYERS 4
#define INFINITU_CYCLE 1
#define DEFAULT 0

#define TRUE 1
#define FALSE 0


static struct ConnectInfo{
    int id;
    int status;
    int sock_tcp;
    struct sockaddr_in from;
};

static uint8_t player_count = DEFAULT;
static uint8_t connects_count = DEFAULT;

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

    return DEFAULT;
}

static int RunServer()
{
    int i,bv;
    int counter;
    int sock_udp;

    uint8_t bufer[5];

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

    while(INFINITU_CYCLE)
    {
    
        from_len = sizeof(struct sockaddr_in);
        bv = recvfrom(sock_udp,bufer,sizeof(bufer),FALSE,(struct sockaddr *)
                &(playerInfo[DEFAULT].from),&from_len);
        if(bv>FALSE)
        {
            printf("get message::");
            for(i=DEFAULT;i<sizeof(bufer);++i)
            {
                printf("%d;",bufer[i]);
            }
            printf("\n");
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
