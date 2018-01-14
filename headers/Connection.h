#ifndef PROJECTBOOM_CONNECTION_H
#define PROJECTBOOM_CONNECTION_H
#include <vector>
#include <chrono>
#include <algorithm>
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
#include <iostream>
#include "Map.h"
#include "Player.h"

using namespace std;

class Connection {
    public: 
    sockaddr_in address;
    Player* player;
    Map* map;
    long lastReceiveTime;
    Connection(sockaddr_in* address);
    void setNowLastReceive();
};

#endif