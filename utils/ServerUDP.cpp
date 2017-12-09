//
// Created by grzegorz on 18.11.17.
//

#include <chrono>
#include "ServerUDP.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "Configuration.h"


ssize_t sendto(int i, char string[50], int i1, int i2);
void sendBombs(int socket, Map* map, sockaddr_in addr);
void sendPlayers(int socket, Map* map, sockaddr_in client);
void sendObstacles(int socket, Map* map, sockaddr_in client);
void probeRequest(int socket, Map* map, sockaddr_in clientAddr, char tab[]);
string deserializeProbeRequest(char tab[]);
void sendMapForAllPlayers(int socket, Map* map);
void sendPong(int socket, sockaddr_in clientAddr, Map* map, char buffer[]);

void deserializeMove(char tab[], Map* map, sockaddr_in clientAddr);


int connection(Map* map )
{
    int nSocket;
    int nBind;
    int nFoo = 1;
    socklen_t nTmp;
    struct sockaddr_in stAddr, stClientAddr;


    /* address structure */
    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(SERVER_PORT);

    /* create a socket */
    nSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (nSocket < 0)
    {
        fprintf(stderr, "Can't create a socket.\n");
        exit(1);
    }

    setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

    //non blocking
//    struct timeval read_timeout;
//    read_timeout.tv_sec = 0;
//    read_timeout.tv_usec = 10;
//    setsockopt(nSocket, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
    /* bind a name to a socket */
    nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if (nBind < 0)
    {
        fprintf(stderr," Can't bind a name to a socket.\n");
        exit(1);
    }


    while(1)
    {
        nTmp = sizeof(struct sockaddr);
        int n = 500;
        char buffer[n];
        if (map->checkAllPlayersHaveName()) {
            //todo odpowiedzi gracza
            sendMapForAllPlayers(nSocket, map);
        }
        if(recvfrom(nSocket, buffer, n, 0,(struct sockaddr*)&stClientAddr, &nTmp) > 0)
            if (buffer[0] == 'p' && buffer[1] == 'r') {
                probeRequest(nSocket, map, stClientAddr, buffer);
            }
            if (buffer[0] == 'p' && buffer[1] == 'i') {
                sendPong(nSocket, stClientAddr, map, buffer);
            }

            if (buffer[0] == 'b' && buffer[1] == 'm') {
                cout<<buffer<<endl;
                deserializeMove(buffer, map, stClientAddr);
            }


    }

}

void sendBombs(int socket, Map* map, sockaddr_in clientAddr){
    string o = serializeBombs(map);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
    return;
}

void sendObstacles(int socket, Map* map, sockaddr_in clientAddr){
    string o = serializeObstacles(map);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
    return;
}

void sendPlayers(int socket, Map* map, sockaddr_in clientAddr){
    string o = serializePlayers(map);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
    return;
}

void probeRequest(int socket, Map* map, sockaddr_in clientAddr, char tab[]){
    string name = deserializeProbeRequest(tab);
    string o;
    if(!map->checkIsOnPlayersList(name)){
        if(!map->addPlayersNameToList(name, &clientAddr)){
            o = "pr:-2";
            char buffer[o.length()];
            strcpy(buffer, o.c_str());
            sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
            return;
        };
    }
    if(!map->checkAllPlayersHaveName()){
        o = "pr:-1";
        char buffer[o.length()];
        strcpy(buffer, o.c_str());
        sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
        return;
    }
    else{
        o = serializeToTableOfPlayers(map);
        char buffer[o.length()];
        strcpy(buffer, o.c_str());
        sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
        return;
    }
}

// different thread
void sendMapForAllPlayers(int socket, Map* map){
        for (int i = 0; i < map->players.size(); i++) {
            sendPlayers(socket, map, map->players.at(i)->socket);
            sendObstacles(socket, map, map->players.at(i)->socket);
            sendBombs(socket, map, map->players.at(i)->socket);
        }
}

void sendPong(int socket, sockaddr_in clientAddr, Map* map, char buffer[]){
    map->setPlayerTimeResponse(&clientAddr);
    sendto(socket, buffer, strlen(buffer), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
    return;
}

