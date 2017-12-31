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
#include "../headers/Player.h"
#include "../map-generator/MapGenerator.h"
#include "../game-judge/GameJudge.h"

int startServer();
void sendBombs(int socket, Map* map, sockaddr_in client);
void sendPlayers(int socket, Map* map, sockaddr_in client);
void sendObstacles(int socket, Map* map, sockaddr_in client);
void sendMapForAllPlayers(int socket, Map* map);
void sendPong(int socket, sockaddr_in clientAddr, Map* map, char tab[]);
#endif //PROJECTBOOM_SERVERUDP_H
