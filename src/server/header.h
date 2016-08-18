#ifndef _HEADER_H_
#define _HEADER_H_

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <sys/time.h>

#include "../include/common.h"
#include "../include/stats_table.h"
#include "../include/action_table.h"
#include "../include/client_server.h"

#include "../logic/logic.h"

#define PORT_UDP        4444
#define PORT_TCP_S      4445 
#define PORT_TCP_CLIENT 9999
#define SERVER_IP       "25.59.28.123"

#define TCP_CONNECTS 5
#define MAX_PLAYERS  4
#define MIN_PLAYERS  2

#define MAKE_TCP_SOCK 1
#define MAKE_UDP_SOCK 2

#define TICK_TO_START   6
#define INFINITU_CYCLE  1
#define DEFAULT         0  /* Default value. */
#define CONNECT_TRY_MAX 5 
#define ONE_TICK_TIME   500  /* Time between one Update in mSec. */

#define TRUE  1
#define FALSE 0

#define ONLINE  1
#define OFFLINE 0

#endif
