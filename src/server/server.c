#ifndef _SERVER_C_
#define _SERVER_C_

#include "header.h"

#define EPOLL_QUEUE_LEN 1
#define PORT_UDP 4444
#define PORT_TCP_S 4445 
#define SERVER_IP "192.168.2.30"

#define TRUE 1
#define FALSE 0


struct ConnectInfo{
    int id;
    int status;
    int sock_tcp;
    struct sockaddr_in server;
};

static int player_count = 0;

static int get_id()
{
    return(player_count++);
}

static int make_socket_non_blocking (int sock)
{
    int flags, s;
    flags = fcntl (sock, F_GETFL, 0);
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

    return 0;
}

static int run_server()
{
    int i,bv;
    int counter;
    int sock_udp;
    struct sockaddr_in server;
    struct ConnectInfo playerInfo[4];
    struct Field field;
    struct epoll_event ev, *events;
    socklen_t server_len;
    
    for(i=0;i<4;++i)
    {
        playerInfo[i].id = get_id();
        playerInfo[i].status = FALSE;
        playerInfo[i].sock_tcp = socket(AF_INET,SOCK_STREAM,0);
        if(playerInfo[i].sock_tcp <=0)
        {
            printf("%d::",i);
            perror("can't make socket::");
            return 1;
        }
        printf("new socket is:%d \n",playerInfo[i].sock_tcp);

    }
    sock_udp = socket(AF_INET,SOCK_DGRAM,0);
   
    if(sock_udp <= 0)
    {
        printf("can't make socket\n");
        return 1;
    }
    
    make_socket_non_blocking(sock_udp);
    
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_UDP);
    
    server_len = sizeof(struct sockaddr_in);    

    bv = bind(sock_udp, (struct sockaddr*)&server,server_len);
    if(bv < 0)
    {
        perror("can't bind someone:");
        return 1;
    }

    

    printf("i'm work\n");
    return 0;
}


int main()
{
    run_server();
    return 0;
}

#endif
