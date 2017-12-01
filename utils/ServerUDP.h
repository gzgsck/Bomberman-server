//
// Created by grzegorz on 18.11.17.
//

#ifndef PROJECTBOOM_SERVERUDP_H
#define PROJECTBOOM_SERVERUDP_H


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include "../headers/Map.h"

#define SERVER_PORT 1234
#define QUEUE_SIZE 5


int connection(Map* map);
#endif //PROJECTBOOM_SERVERUDP_H
