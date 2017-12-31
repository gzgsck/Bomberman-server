//
// Created by grzegorz on 18.11.17.
//
#include <vector>
#include <chrono>
#include <algorithm>
#include <unistd.h>
#include "ServerUDP.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "Configuration.h"

class LookupRecord {
    public:
        int lastAccess;
        char request[500];
        int requestLength;
        char response[500];
        int responseLength;
        sockaddr_in address;

    
    LookupRecord(char request[], int requestLength,  char response[], int responseLength, sockaddr_in* address) {
        lastAccess = chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        
         memset(&this->address, 0, sizeof(struct sockaddr_in));
        this->address.sin_addr = address->sin_addr;
        this->address.sin_addr.s_addr = address->sin_addr.s_addr;
        this->address.sin_family = address->sin_family;
        this->address.sin_port = address->sin_port;

        strncpy(this->request, request, requestLength);
        strncpy(this->response, response, responseLength);
        this->requestLength = requestLength;
        this->responseLength = responseLength;

        
    }

    bool isSame(char retryRequest[], int retryRequestLength, sockaddr_in* address) {
        if (retryRequestLength - 2 != this->requestLength) return false;

        if (retryRequest[2] != this->request[0] ||
            retryRequest[3] != this->request[1]
        ) return false; 
        

        if (address->sin_addr.s_addr != this->address.sin_addr.s_addr ||
            address->sin_port != this->address.sin_port) return false;

        printf("%s\n",retryRequest+4);
        printf("%s\n", this->request + 2);

        return strcmp(retryRequest + 4, this->request + 2) == 0;
    }

    bool isOutdated() {
        int now = chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

        return lastAccess + 1000 < now;
    }

    void copyResponse(char destination[]) {
        lastAccess = chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        strncpy(destination, this->response, this->responseLength);
    }
};

class LookupTable {
    public:
    vector<LookupRecord*> records = {};

    void add(char request[], int requestLength,  char response[], int responseLength, sockaddr_in* address) {
        records.push_back(new LookupRecord(request, requestLength, response, responseLength, address));
    }

    void search(char retryRequest[], int retryRequestLength, char response[], sockaddr_in* address) {
        for (LookupRecord* record: records) {
            if (record->isSame(retryRequest, retryRequestLength, address)) {
                cout << "Found record: " << record->responseLength << endl;
                record->copyResponse(response);
                response[record->responseLength] = '\0';
                return;
            }
        }
        response[0] = '\0';
    }

    void removeOutdated() {
        vector<LookupRecord*> newRecords;
        auto it = std::remove_copy_if(records.begin(), records.end(), std::back_inserter(newRecords), [](LookupRecord* record) {
            return !record->isOutdated();
        });
        this->records = newRecords;

    }
};

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

ssize_t sendto(int i, char string[50], int i1, int i2);
void sendBombs(int socket, Map* map, sockaddr_in addr);
void sendPlayers(int socket, Map* map, sockaddr_in client);
void sendObstacles(int socket, Map* map, sockaddr_in client);
void probeRequest(int socket, Connection* connection, char message[], int messageLength);
void sendMapForAllPlayers(int socket, Map* map);
void sendPong(int socket, sockaddr_in clientAddr, char buffer[]);
void handleConnected(Connection* connection, int serverSocket, char message[], int messageLength);
void handleNotConnected(int serverSocket, sockaddr_in clientAddr, char message[], int messageLength);

std::vector<Map*> pendingGames = {};
std::vector<Map*> allGames = {};

std::vector<Connection*> connections = {};
LookupTable* lt = new LookupTable();

Map* findGameById(int id) {
    for (Map* map : pendingGames) {
        if (map->id == id) return map;
    }
    return nullptr;
}

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

void* singleGameRoutine(void* param) {
    // int mapId = *((int*) param);
    // Map *map = find_if(allGames.begin(), allGames.end(), [&mapId](Map* m) {
    //     return m->id = mapId;
    // });
    Map *map = (Map*) param;
    cout << "Starting thread for map:" << map->id << endl;
    while(true) {
        usleep(200000);
        cout << "singleGameRoutine" << map->id << endl;
    }
}
// void* singleGameManager(void* params) {
//     while(true) {
//         sleep(0.5);
//         manageBombsExplosions(map);
//         manageFires(map);
//         managePlayers(map);
//     }
// }

int idSequence = 0;
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
            if (buffer[0] == 'r' && buffer[1] == 't') {
                char response[500];
                lt->search(buffer, length, response, &stClientAddr);
                int responseLength = strlen(response);
                cout << "Retransmision request: " << responseLength << endl;
                if (responseLength > 0) {
                    sendto(serverSocket, response, responseLength, 0, (struct sockaddr*)&stClientAddr, sizeof(stClientAddr));
                } else {
                    Connection* incomming = existConnectionForAddress(&stClientAddr);
                    if (incomming != nullptr) {
                        handleConnected(incomming, serverSocket, buffer, length);
                    } else {
                        handleNotConnected(serverSocket, stClientAddr, buffer, length);
                    }
                }
            
            } else {
                Connection* incomming = existConnectionForAddress(&stClientAddr);

                if (incomming != nullptr) {
                    handleConnected(incomming, serverSocket, buffer, length);
                } else {
                    handleNotConnected(serverSocket, stClientAddr, buffer, length);
                }
            }
        }
    }
}

void getAcknownladge(int sequenceLength, int sequenceStart, char message[], char ackPacket[]) {
    ackPacket[0] = 'a';
    ackPacket[1] = 'c';
    strncpy(ackPacket + 2, message + sequenceStart, sequenceLength);
    ackPacket[sequenceLength + 2] = '\0';
}

void handleConnected(Connection* connection, int serverSocket, char message[], int messageLength) {
    connection->setNowLastReceive();
    sockaddr_in clientAddr = connection->address;
  
    if (message[0] == 'p' && message[1] == 'i') {
        sendPong(serverSocket, connection->address, message);
    }
    if (message[0] == 'p' && message[1] == 'r') {
        probeRequest(serverSocket, connection, message, messageLength);
    }

    Map *map = connection->map;
    Player *player = connection->player;

    if (message[0] == 'm' && message[1] == 'v') {
        if (connection->map == nullptr || connection->player == nullptr) {
            cout << "Receiving bomb request but not active game for connection" << endl;
        } else {
            deserializeMove(message, map, player);
        }
    }
    if (message[0] == 'b' && message[1] == 'm') {
        if (connection->map == nullptr || connection->player == nullptr) {
            cout << "Receiving move request but not active game for connection" << endl;
        } else {
            deserializeBomb(message, map, player);
        }
    }
}

void handleNotConnected(int serverSocket, sockaddr_in clientAddr, char message[], int messageLength) {
    if (message[0] == 'c' && message[1] == 'n') {
        connections.push_back(new Connection(&clientAddr));
        char response[500];
        getAcknownladge(20, 2, message, response);
        int responseLength = strlen(response);

        lt->add(message, messageLength, response, responseLength, &clientAddr);

        sendto(serverSocket, response, responseLength, 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
    } else {
        cout << "Reciving message different then connection request from not connected client" << endl; 
    }
}

void sendBombs(int socket, Map* map, sockaddr_in clientAddr) {
    string o = serializeBombs(map);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
    return;
}

void sendObstacles(int socket, Map* map, sockaddr_in clientAddr) {
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

bool createGame(Connection* connection, string name) {
    if (connection->map != nullptr) return false;
    Map* map = generateMap();
    map->id = ++idSequence;
    map->status = "pending";
    int playerId = map->addPlayersNameToList(name);

    connection->player = map->players.at(playerId);
    connection->map = map;
    pendingGames.push_back(map);
    allGames.push_back(map);
    cout << "Starting thread for: " << map->id << endl;
    pthread_t gameThread;
    
return pthread_create(&gameThread, 0, &singleGameRoutine, map) == 0;
}

bool joinGame(Connection* connection, string name, int id) {
    if (connection->map != nullptr) return false;

    Map* map = findGameById(id);
    if (map == nullptr) return false;
    if (map->checkIsOnPlayersList(name) != -1) return false;
    int playerId = map->addPlayersNameToList(name);
    connection->player = map->players.at(playerId);
    connection->map = map;

    if (map->checkAllPlayersHaveName()) {
        map->status = "inprogress";
        pendingGames.erase(std::remove(pendingGames.begin(), pendingGames.end(), map), pendingGames.end());
    }

    return true;

}

void probeRequest(int socket, Connection* connection, char message[], int messageLength) {
    string name = deserializeName(message);
    int id = deserializeMapId(message);
    int size = ((int)message[2] - 40);
    sockaddr_in client = connection->address;

    cout << name << " requested join to game " << id << endl;

    if (id < 0) {
        bool success = createGame(connection, name);
        if (success) {
            char response[500];
            getAcknownladge(20, 2, message, response);  
            int responseLength = strlen(response);

            lt->add(message, messageLength, response, responseLength, &client);

            sendto(socket, response, responseLength, 0, (struct sockaddr*)&client, sizeof(client));
        } else {
            cout << "Not allowed" << endl;
        }
    } else {
        bool success = joinGame(connection, name, id);
        if (success) {
            char response[500];
            getAcknownladge(20, 2, message, response);
            int responseLength = strlen(response);

            lt->add(message, messageLength, response, responseLength, &client);

            sendto(socket, response, responseLength, 0,(struct sockaddr*)&client, sizeof(client));
        } else {
            cout << "Not allowed" << endl;
        }
    }
    // string o;
    // int idx = map->checkIsOnPlayersList(name);
    // if (idx == -1) {
    //     idx = map->addPlayersNameToList(name);
    //     if (idx == -1) {
    //         o = "pr:-2";
    //         char buffer[o.length()];
    //         strcpy(buffer, o.c_str());
    //         sendto(socket, buffer, o.length(), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
    //         return;
    //     } else {

    //     }
    // } else {
    //     // if (map->players.at(idx)->socket.sin_addr.s_addr == clientAddr.sin_addr.s_addr) {
    //     //     map->players.at(idx)->setSocket(&clientAddr);
    //     // }
    // }

    // Player* player = map->players.at(idx);

    // int allPlayersReady = map->checkAllPlayersHaveName();
    // int gameStatus = allPlayersReady ? 0 : -1;

    // o = serializeToTableOfPlayers(map, -1, player);
    // char buffer[o.length()];
    // strcpy(buffer, o.c_str());
    // sendto(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
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

