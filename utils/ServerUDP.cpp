//
// Created by grzegorz on 18.11.17.
//
#include <vector>
#include "../headers/LookupTable.h"
#include "../headers/Connection.h"
#include "../headers/Semaphore.h"

#include "ServerUDP.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "Configuration.h"


void sendBombs(int socket, Map* map, sockaddr_in client);
void sendPlayers(int socket, Map* map, sockaddr_in client);
void sendObstacles(int socket, Map* map, sockaddr_in client);
void probeRequest(int socket, Connection* connection, char message[], int messageLength);
void sendMapForAllPlayers(int socket, Map* map);
void sendPong(int socket, sockaddr_in clientAddr, char buffer[]);
void handleConnected(Connection* connection, int serverSocket, char message[], int messageLength);
void handleNotConnected(int serverSocket, sockaddr_in clientAddr, char message[], int messageLength);

std::vector<Map*> pendingGames = {};
std::vector<Map*> allGames = {};

pthread_mutex_t socketMutex = PTHREAD_MUTEX_INITIALIZER;
int connectionsSemaphore;
std::vector<Connection*> connections = {};
int serverSocket;

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

ssize_t sendOnSocket(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) {
    pthread_mutex_lock(&socketMutex);
    sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    pthread_mutex_unlock(&socketMutex);
}

void* singleGameRoutine(void* param) {
    Map *map = (Map*) param;
    while(true) {
        usleep(200000);
        cout << "singleGameRoutine " << map->id << " starts" << endl;
        lowerSemaphore(map->semaphore, 0, 1);
        lowerSemaphore(connectionsSemaphore, 0, 1);

        manageBombsExplosions(map);
        manageFires(map);
        managePlayers(map);

        
        for (int i = 0; i < map->players.size(); i++) {
            Player* player = map->players.at(i);
            if (player->name.size() > 0) {
                int gameStatus =  map->checkAllPlayersHaveName() ? 0 : -1;
                
                struct sockaddr_in* address = (struct sockaddr_in*)&(player->connection->address);
                socklen_t len = sizeof(sockaddr);
                string players = serializeToTableOfPlayers(map, gameStatus, player);
                char buffer[players.length()];
                strcpy(buffer, players.c_str());

                int sendLength = sendOnSocket(serverSocket, buffer, players.length(), 0, (struct sockaddr*)address, len);
                if (sendLength == -1) {
                    perror("Could not send");
                }
            }
        }

        if (map->status.compare("inprogress") == 0) {
            cout << map->status << endl;
            for (int i = 0; i < map->players.size(); i++) {
                sendPlayers(serverSocket, map, map->players.at(i)->connection->address);
                sendObstacles(serverSocket, map, map->players.at(i)->connection->address);
                sendBombs(serverSocket, map, map->players.at(i)->connection->address);
            }
        }
        
        raiseSemaphore(connectionsSemaphore, 0, 1);
        raiseSemaphore(map->semaphore, 0, 1);
        cout << "singleGameRoutine " << map->id << " ends" << endl;

    }
}

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
    serverSocket = createSocket();
    struct sockaddr_in stClientAddr;
    socklen_t structureSize = sizeof(struct sockaddr);
    int bufferSize = 500;

    connectionsSemaphore = semget(IPC_PRIVATE, 1, IPC_CREAT|0600);
    if (semctl(connectionsSemaphore, 0, SETVAL, (int)500) == -1) {
      perror("Nadanie wartosci semaforowi conneciton list");
      exit(1);
    }
    
    cout << "Server socket " << serverSocket << endl;
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
                    sendOnSocket(serverSocket, response, responseLength, 0, (struct sockaddr*)&stClientAddr, sizeof(stClientAddr));
                } else {

                    Connection* incomming = existConnectionForAddress(&stClientAddr);
                    if (incomming != nullptr) {
                        lowerSemaphore(connectionsSemaphore, 0, 500);
                        handleConnected(incomming, serverSocket, buffer, length);
                        raiseSemaphore(connectionsSemaphore, 0, 500);
                    } else {
                        handleNotConnected(serverSocket, stClientAddr, buffer, length);
                    }
                }
            
            } else {
                Connection* incomming = existConnectionForAddress(&stClientAddr);

                if (incomming != nullptr) {
                    lowerSemaphore(connectionsSemaphore, 0, 500);
                    handleConnected(incomming, serverSocket, buffer, length);
                    raiseSemaphore(connectionsSemaphore, 0, 500);
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

        sendOnSocket(serverSocket, response, responseLength, 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
    } else {
        cout << "Reciving message different then connection request from not connected client" << endl; 
    }
}

void sendBombs(int socket, Map* map, sockaddr_in clientAddr) {
    string o = serializeBombs(map);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendOnSocket(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
    return;
}

void sendObstacles(int socket, Map* map, sockaddr_in clientAddr) {
    string o = serializeObstacles(map);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendOnSocket(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
    return;
}

void sendPlayers(int socket, Map* map, sockaddr_in clientAddr) {
    string o = serializePlayers(map);
    char buffer[o.length()];
    strcpy(buffer, o.c_str());
    sendOnSocket(socket, buffer, o.length(), 0,(struct sockaddr*)&clientAddr, sizeof(clientAddr));
    return;
}

bool createGame(Connection* connection, string name) {
    if (connection->map != nullptr) return false;
    Map* map = generateMap();
    map->id = ++idSequence;
    map->status = "pending";
    int playerId = map->addPlayersNameToList(name);

    connection->player = map->players.at(playerId);
    map->players.at(playerId)->setConnection(connection);
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
    lowerSemaphore(map->semaphore, 0, 1);
    if (map->checkIsOnPlayersList(name) != -1) return false;
    int playerId = map->addPlayersNameToList(name);

    connection->player = map->players.at(playerId);
    map->players.at(playerId)->setConnection(connection);
    connection->map = map;

    if (map->checkAllPlayersHaveName()) {
        map->status = "inprogress";
        pendingGames.erase(std::remove(pendingGames.begin(), pendingGames.end(), map), pendingGames.end());
    }
    raiseSemaphore(map->semaphore, 0, 1);
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

            sendOnSocket(socket, response, responseLength, 0, (struct sockaddr*)&client, sizeof(client));
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

            sendOnSocket(socket, response, responseLength, 0,(struct sockaddr*)&client, sizeof(client));
        } else {
            cout << "Not allowed" << endl;
        }
    }

}

void sendPong(int socket, sockaddr_in clientAddr, char buffer[]) {
    cout << " Address: " << inet_ntoa(clientAddr.sin_addr) << " port: " << clientAddr.sin_port;
    sendOnSocket(socket, buffer, strlen(buffer), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
    return;
}

