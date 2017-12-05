//
// Created by grzegorz on 18.11.17.
//

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
        recvfrom(nSocket, buffer, n, 0,(struct sockaddr*)&stClientAddr, &nTmp);
        if(buffer[0] == 'p' && buffer[1] == 'r'){
            //probe request
            probeRequest(nSocket, map, stClientAddr, buffer);





            if(map->checkAllPlayersHaveName()) {
                sendMapForAllPlayers(nSocket, map);
            }
        }
        //todo odpowiedzi gracza
        if(buffer[0] == 'a' && buffer[1] == 'b'){

        }




//        sendBombs(nSocket, map, stClientAddr);
//        sendPlayers(nSocket, map, stClientAddr);
//        sendObstacles(nSocket, map, stClientAddr);


    }

}

void sendBombs(int socket, Map* map, sockaddr_in clientAddr){
    string o = serializeBombs(map);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
}

void sendObstacles(int socket, Map* map, sockaddr_in clientAddr){
    string o = serializeObstacles(map);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
}

void sendPlayers(int socket, Map* map, sockaddr_in clientAddr){
    string o = serializePlayers(map);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
}

void probeRequest(int socket, Map* map, sockaddr_in clientAddr, char tab[]){
    string name = deserializeProbeRequest(tab);
    string o;
    if(!map->checkIsOnPlayersList(name)){
        if(!map->addPlayersNameToList(name, &clientAddr)){
            cout<<"okok"<<endl;
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
    for(int i = 0 ; i< map->players.size(); i++){
        sendPlayers(socket, map, *map->players.at(i)->socket);
        sendObstacles(socket, map, *map->players.at(i)->socket);
        sendBombs(socket, map, *map->players.at(i)->socket);
    }
}



