//
// Created by grzegorz on 18.11.17.
//
#include <vector>
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
void sendMapForAllPlayers(int socket, Map* map);
void sendPong(int socket, sockaddr_in clientAddr, char buffer[]);

class Connection {
    public: 
        sockaddr_in address;
        Player* player;
        Map* map;
        int lastReceiveTime;

        Connection(sockaddr_in* address) {
            memset(&this->address, 0, sizeof(struct sockaddr_in));
            this->address.sin_addr = address->sin_addr;
            this->address.sin_addr.s_addr = address->sin_addr.s_addr;
            this->address.sin_family = address->sin_family;
            this->address.sin_port = address->sin_port;
            map = nullptr;
            player = nullptr;
        }
        
        void setNowLastReceive() {
            lastReceiveTime = chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        }
   
};

void handleConnected(Connection* connection, int serverSocket, char message[]);
void handleNotConnected(int serverSocket, sockaddr_in clientAddr, char message[]);

std::vector<Map*> maps = {};
std::vector<Connection*> connections = {};



Connection* existConnectionForName(string name) {
    for (Connection* connection : connections) {
        bool exist = connection->player->name == name;
        if (exist) return connection;
    }
    return nullptr;
}

Connection* existConnectionForAddress(sockaddr_in* incomming) {
    for (Connection* connection : connections) {
        if (connection->address.sin_addr.s_addr == incomming->sin_addr.s_addr &&
            connection->address.sin_port == incomming->sin_port
        ){
            return connection;
        }
    }
    return nullptr;
    
}


// void* singleGameManager(void* params) {
//     while(true) {
//         sleep(0.5);
//         manageBombsExplosions(map);
//         manageFires(map);
//         managePlayers(map);
//     }
// }

int createSocket() {
    int nSocket;
    int nBind;
    int nFoo = 1;
    struct sockaddr_in stAddr;


    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(SERVER_PORT);

    nSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (nSocket < 0)
    {
        fprintf(stderr, "Can't create a socket.\n");
        exit(1);
    }
    cout << "Server running on port: " << SERVER_PORT << endl;

    setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    setsockopt(nSocket, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

    nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if (nBind < 0)
    {
        fprintf(stderr," Can't bind a name to a socket.\n");
        exit(1);
    }
    return nSocket;
}

int startServer() {
    int serverSocket = createSocket();
    struct sockaddr_in stClientAddr;
    socklen_t structureSize = sizeof(struct sockaddr);
    int bufferSize = 500;


    while(1)
    {   
        char buffer[bufferSize];

        int length = recvfrom(serverSocket, buffer, bufferSize, 0, (struct sockaddr*)&stClientAddr, &structureSize);

        if (length > 0) {
            Connection* incomming = existConnectionForAddress(&stClientAddr);

            if (incomming != nullptr) {
                handleConnected(incomming, serverSocket, buffer);
            } else {
                handleNotConnected(serverSocket, stClientAddr, buffer);
            }
        }
    }
}

void sendAcknownladge(int serverSocket, int sequenceLength, int sequenceStart, char message[], sockaddr_in clientAddress) {
    char uniqueSequence[22];
    uniqueSequence[0] = 'a';
    uniqueSequence[1] = 'c';
    strncpy(uniqueSequence + 2, message + sequenceStart, sequenceLength);
    sendto(
        serverSocket,
        uniqueSequence,
        sequenceLength + 2,
        0,
        (struct sockaddr*)&clientAddress,
        sizeof(clientAddress)
    );
}

void handleConnected(Connection* connection, int serverSocket, char message[]) {
    connection->setNowLastReceive();
    sockaddr_in clientAddr = connection->address;
  
    if (message[0] == 'p' && message[1] == 'i') {
        sendPong(serverSocket, connection->address, message);
    }


    Map *map = connection->map;
    Player *player = connection->player;

    if (message[0] == 'p' && message[1] == 'r') {
        probeRequest(serverSocket, map, clientAddr, message);
    }
    if (message[0] == 'm' && message[1] == 'v') {
        deserializeMove(message, map, player);
    }
    if (message[0] == 'b' && message[1] == 'm') {
        deserializeBomb(message, map, player);
    }
}

void handleNotConnected(int serverSocket, sockaddr_in clientAddr, char message[]) {
    if (message[0] == 'c' && message[1] == 'n') {
        connections.push_back(new Connection(&clientAddr));
        sendAcknownladge(serverSocket, 20, 2, message, clientAddr);
    } else {
        cout << "Reciving message different then connection request from not connected client" << endl; 
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

void sendPlayers(int socket, Map* map, Connection *connection) {
    sockaddr_in clientAddr = connection->address;
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
        idx = map->addPlayersNameToList(name);
        if (idx == -1) {
            o = "pr:-2";
            char buffer[o.length()];
            strcpy(buffer, o.c_str());
            sendto(socket, buffer, o.length(), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
            return;
        } else {

        }
    } else {
        // if (map->players.at(idx)->socket.sin_addr.s_addr == clientAddr.sin_addr.s_addr) {
        //     map->players.at(idx)->setSocket(&clientAddr);
        // }
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
// void sendMapForAllPlayers(int socket, Map* map){
//     for (int i = 0; i < map->players.size(); i++) {
//         sendPlayers(socket, map, map->players.at(i)->socket);
//         sendObstacles(socket, map, map->players.at(i)->socket);
//         sendBombs(socket, map, map->players.at(i)->socket);
//     }
// }

void sendPong(int socket, sockaddr_in clientAddr, char buffer[]) {
    sendto(socket, buffer, strlen(buffer), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
    return;
}

