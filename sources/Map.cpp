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

int Map::addPlayersNameToList(string name) {
    if (checkAllPlayersHaveName()) {
        return -1;
    }
    for(int i = 0; i < this->players.size(); i++){
        if(this->players.at(i)->name.size()<1){
            this->players.at(i)->name = name;
            return i;
        }
    }
}

void Map::setPlayerMove(Player* player, int x, int y) {
    player->resetPosition = !this->canMoveTo(player, x, y);

    if (player->resetPosition) {
        return;
    }

    Cell* cell = getCellByPosition(x/MAP_FIELD_SIZE, y/MAP_FIELD_SIZE);
    if (cell->fire != nullptr) {
        if (!player->isProtected) {
            player->lifes -= 1;

            if (player->lifes < 1) {
                player->isAlive = false;
            }
            player->setProtection();
        }
    }

    player->position->y = y;
    player->position->x = x;
}

bool Map::canMoveTo(Player* player, int x , int y) {
    Cell* cell = getCellByPosition(x/MAP_FIELD_SIZE, y/MAP_FIELD_SIZE);
    if (player->position->x/MAP_FIELD_SIZE == x/MAP_FIELD_SIZE && player->position->y/MAP_FIELD_SIZE == y/MAP_FIELD_SIZE){
       return true;
    }
    if (cell->obstacle != nullptr || cell->bomb != nullptr){
        return false;
    }
    return true;
}

void Map::setBombPlant(Player* player, int x, int y){
    Cell* cell = getCellByPosition(x, y);
    if(canPlantBomb(player, cell) == false){
        return;
    }
    Bomb* bomb = new Bomb(player, player->bombPower);
    player->plantBomb(bomb);
    bomb->owner = player;
    cell->bomb = bomb;
}

bool Map::canPlantBomb(Player* player, Cell* cell){
    if(player->avaliableBombs - player->bombs.size() <= 0){
        cout << "No more bombs" << endl;
        return false;
    }
    if(cell->bomb != nullptr){
        cout << "Already bomb" << endl;
        return false;
    }
    if(cell->obstacle != nullptr){
        return false;
    }
    return true;
}

Cell* Map::getCellByPosition(int x, int y){
    return this-> cells[y][x];
}
