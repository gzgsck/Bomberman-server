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
void deserializeBomb(char buffer[], Map* map, sockaddr_in clientAddr);
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
    cout << "Server running on port: " << SERVER_PORT << endl;

    setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

    //non blocking
   struct timeval read_timeout;
   read_timeout.tv_sec = 0;
   read_timeout.tv_usec = 10;
   setsockopt(nSocket, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
    /* bind a name to a socket */
    nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if (nBind < 0)
    {
        fprintf(stderr," Can't bind a name to a socket.\n");
        exit(1);
    }

    int iter = 0; 
    while(1)
    {
        nTmp = sizeof(struct sockaddr);
        int n = 500;
        char buffer[n];
        
        if (map->checkAllPlayersHaveName()) {
            //todo odpowiedzi gracza
            sendMapForAllPlayers(nSocket, map);
        }
        if (iter % 100 == 0) {
            for (int i = 0; i < map->players.size(); i++) {
                Player* player = map->players.at(i);
                if (player->name.size() > 0) {
                    int gameStatus =  map->checkAllPlayersHaveName() ? 0 : -1;
                    
                    struct sockaddr* address = (struct sockaddr*)&player->socket;
                    socklen_t len = sizeof(player->socket);
                    string o = serializeToTableOfPlayers(map, gameStatus, player);
                    char buffer[o.length()];
                    strcpy(buffer, o.c_str());
                    sendto(nSocket, buffer, o.length(), 0, address, len);
                }
            }
        }
        
        int length = recvfrom(nSocket, buffer, n, 0, (struct sockaddr*)&stClientAddr, &nTmp);
        if (length > 0) {
            if (buffer[0] == 'p' && buffer[1] == 'r') {
                probeRequest(nSocket, map, stClientAddr, buffer);
            }
            if (buffer[0] == 'p' && buffer[1] == 'i') {
                sendPong(nSocket, stClientAddr, map, buffer);
            }

            if (buffer[0] == 'm' && buffer[1] == 'v') {
                cout<<buffer<<endl;
                deserializeMove(buffer, map, stClientAddr);
            }
            if (buffer[0] == 'b' && buffer[1] == 'm') {
                cout<<buffer<<endl;
                deserializeBomb(buffer, map, stClientAddr);
            }
        }
        iter = (iter + 1) % 10000;
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
    int idx = map->checkIsOnPlayersList(name);
    if (idx == -1) {
        idx = map->addPlayersNameToList(name, &clientAddr);
        if (idx == -1) {
            o = "pr:-2";
            char buffer[o.length()];
            strcpy(buffer, o.c_str());
            sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
            return;
        };
    } else {
        if (map->players.at(idx)->socket.sin_addr.s_addr == clientAddr.sin_addr.s_addr) {
            map->players.at(idx)->setSocket(&clientAddr);
        }
    }

    Player* player = map->players.at(idx);

    int allPlayersReady = map->checkAllPlayersHaveName();
    int gameStatus = allPlayersReady ? 0 : -1;

    o = serializeToTableOfPlayers(map, -1, player);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
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

