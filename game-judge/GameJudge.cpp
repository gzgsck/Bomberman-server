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
                cout<<bomb->timestamp + bomb->durationTime<<"   "<<serverTime<<endl;
                if(bomb->timestamp + bomb->durationTime <= serverTime){
                    searchInRange(map, i, k);
                }
            }
        }
    }
}

void searchInRange(Map* map, int cellX, int cellY){
    Bomb* bomb = map->cells[cellX][cellY]->bomb;
    bomb->owner->avaliableBombs += 1;
    int range = bomb->power;
    int xL = (cellX - range < 0)? 0 : cellX - range;
    int xR = (cellX + range > MAP_SIZE - 1)? MAP_SIZE - 1 : cellX + range;
    int yU = (cellY - range < 0)? 0 : cellX - range;
    int yD = (cellY + range > MAP_SIZE - 1)? MAP_SIZE - 1 : cellX + range;


    map->cells[cellX][cellY]->bomb = nullptr;

    for(xL; xL <= xR; xL++){
        if(map->cells[xL][cellY]->bomb != nullptr){
            searchInRange(map, xL, cellY);
        }
        if(map->cells[xL][cellY]->obstacle != nullptr){
            destroyObstacle(map, xL, cellY);
        }
        killPlayerOnField(map, xL, cellY);
    }

    for(yU; yU <= yD; yU++){
        if(yU == cellY){ continue;}
        if(map->cells[cellX][yU]->bomb != nullptr){
            searchInRange(map, cellX, yU);
        }
        if(map->cells[xL][cellY]->obstacle != nullptr){
            destroyObstacle(map, cellX, yU);
        }
        killPlayerOnField(map, cellX, yU);
    }
}

void destroyObstacle(Map* map, int x, int y){
    if(map->cells[x][y]->obstacle->isDestroyable()){
        map->cells[x][y]->obstacle = nullptr;
    }
}

void killPlayerOnField(Map* map, int x, int y){
    for(int i = 0; i < map->players.size(); i++){
        if(map->players.at(i)->isPlayerOnField(x,y)){
            if(map->players.at(i)->lifes <= 1){
                map->players.at(i)->isAlive = false;
                continue;
            }
            else{
                map->players.at(i)->lifes-=1;
                continue;
            }
        }
    }
}