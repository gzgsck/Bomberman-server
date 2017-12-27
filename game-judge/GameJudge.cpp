//
// Created by grzegorz on 01.12.17.
//

#include <chrono>
#include "GameJudge.h"


void manageBombsExplosions(Map* map) {
    for(int i = 0; i < MAP_SIZE; i++){
        for(int k = 0 ; k < MAP_SIZE; k++){
            if(map->cells[i][k]->bomb == nullptr){ continue;}
            else{
                long serverTime = chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                Bomb* bomb = map->cells[i][k]->bomb;
                if(bomb->timestamp + bomb->durationTime <= serverTime){
                    searchInRange(map, i, k);
                }
            }
        }
    }
}

void manageFires(Map* map) {
    for(int i = 0; i < MAP_SIZE; i++){
        for(int k = 0 ; k < MAP_SIZE; k++){
            if(map->cells[i][k]->fire == nullptr){ continue;}
            else{
                long serverTime = chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                Fire* fire = map->cells[i][k]->fire;
                if (fire->timestamp + fire->durationTime <= serverTime) {
                    delete fire;
                    map->cells[i][k]->fire = nullptr;
                }
            }
        }
    }
}

void searchInRange(Map* map, int cellX, int cellY){
    Bomb* bomb = map->cells[cellX][cellY]->bomb;
    int range = bomb->power;
    int xL = (cellX - range < 0)? 0 : cellX - range;
    int xR = (cellX + range > MAP_SIZE - 1)? MAP_SIZE - 1 : cellX + range;
    int yU = (cellY - range < 0)? 0 : cellY - range;
    int yD = (cellY + range > MAP_SIZE - 1)? MAP_SIZE - 1 : cellY + range;


    map->cells[cellX][cellY]->bomb = nullptr;
    map->cells[cellX][cellY]->fire = new Fire(1000);
    bomb->owner->removeBomb(bomb);

    for (int i = cellX; i <= xR; i++) {
        if (map->cells[i][cellY]->bomb != nullptr) {
            searchInRange(map, i, cellY);
            break;
        }
        if (map->cells[i][cellY]->obstacle != nullptr) {
            destroyObstacle(map, i, cellY);
            break;
        }
        map->cells[i][cellY]->fire = new Fire(1000);

        killPlayerOnField(map, i, cellY);
    }

    for (int i = cellX; i >= xL; i--){
        if (map->cells[i][cellY]->bomb != nullptr){
            searchInRange(map, i, cellY);
            break;
        }
        if (map->cells[i][cellY]->obstacle != nullptr){
            destroyObstacle(map, i, cellY);
            break;
        }
        map->cells[i][cellY]->fire = new Fire(1000);

        killPlayerOnField(map, i, cellY);
    }

    for (int i = cellY; i >= yU; i--){
        if (map->cells[cellX][i]->bomb != nullptr) {
            searchInRange(map, cellX, i);
            break;
        }
        if (map->cells[cellX][i]->obstacle != nullptr) {
            destroyObstacle(map, cellX, i);
            break;
        }
        map->cells[i][cellY]->fire = new Fire(1000);

        killPlayerOnField(map, cellX, i);
    }

    for (int i = cellY; i <= yD; i++){
        if (map->cells[cellX][i]->bomb != nullptr){
            searchInRange(map, cellX, i);
            break;
        }
        if (map->cells[cellX][i]->obstacle != nullptr){
            destroyObstacle(map, cellX, i);
            break;
        }
        map->cells[i][cellY]->fire = new Fire(1000);

        killPlayerOnField(map, cellX, i);
    }
}

void destroyObstacle(Map* map, int x, int y) {
    if (map->cells[x][y]->obstacle->isDestroyable()) {
        map->cells[x][y]->obstacle = nullptr;
    }
}

void killPlayerOnField(Map* map, int row, int col) {
    for (int i = 0; i < map->players.size(); i++) {
        Player *player = map->players.at(i);
        if (player->isProtected) {
            continue;
        }
        if (player->isPlayerOnField(row, col)) {
            player->lifes -= 1;

            if (player->lifes < 1) {
                player->isAlive = false;
            }
            player->isProtected = true;
        }
    }
}