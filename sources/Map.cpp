//
// Created by grzegorz on 16.11.17.
//

#include <netinet/in.h>
#include <chrono>
#include "../headers/Map.h"


bool Map::checkAllPlayersHaveName(){
    for(int i = 0 ; i < this->players.size(); i++){
        if((this->players.at(i)->name.compare("") == 0 )){
            return false;
        }
    }
    return true;
}

int Map::checkIsOnPlayersList(string name){
    for (int i = 0 ; i< this->players.size(); i++){
        if(this->players.at(i)->name.compare(name) == 0){
            return i;
        }
    }
    return -1;
}

int Map::addPlayersNameToList(string name, sockaddr_in* sock){
    if (checkAllPlayersHaveName()) {
        return -1;
    }
    for(int i = 0; i < this->players.size(); i++){
        if(this->players.at(i)->name.size()<1){
            this->players.at(i)->name = name;
            this->players.at(i)->setSocket(sock);
            return i;
        }
    }
}

void Map::setPlayerTimeResponse(sockaddr_in* sock){
    Player* p = findPlayerBySocaddr(sock);
    if(p != nullptr) {
        p->lastResponseTime = chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }
}

Player* Map::findPlayerBySocaddr(sockaddr_in* sock){
    for(int i = 0 ; i < this->players.size(); i++){
        if(this->players.at(i)->socket.sin_addr.s_addr == sock->sin_addr.s_addr && this->players.at(i)->socket.sin_port == sock->sin_port){
            return this->players.at(i);
        }
    }
    return nullptr ;
}


void Map::setPlayerMove(sockaddr_in* sock, int x, int y){
    Player* player = this->findPlayerBySocaddr(sock);
    player->position->y += y;
    player->position->x += x;
}

void Map::setBombPlant(sockaddr_in* sock, int x, int y){
    Player* player = this->findPlayerBySocaddr(sock);
    //todo pozycja gracza czy x,y?
    Cell* cell = getCellByPosition(player->position->x, player->position->y);
    Bomb* bomb = new Bomb(player, player->bombPower);
    bomb->owner = player;
    player->bombs.push_back(bomb);
    cell->bomb = bomb;

}

Cell* Map::getCellByPosition(int x, int y){
    return this-> cells[x/MAP_FIELD_SIZE][y/MAP_FIELD_SIZE];
}