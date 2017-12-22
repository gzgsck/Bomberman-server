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
void searchInRange(Map* map, int cellX, int cellY){
    Bomb* bomb = map->cells[cellX][cellY]->bomb;
    int range = bomb->power;
    int xL = (cellX - range < 0)? 0 : cellX - range;
    int xR = (cellX + range > MAP_SIZE - 1)? MAP_SIZE - 1 : cellX + range;
    int yU = (cellY - range < 0)? 0 : cellY - range;
    int yD = (cellY + range > MAP_SIZE - 1)? MAP_SIZE - 1 : cellY + range;

    pthread_mutex_lock(&map->mutex);
    map->cells[cellX][cellY]->bomb = nullptr;
    bomb->owner->removeBomb(bomb);
    pthread_mutex_unlock(&map->mutex);

    for(int i = cellX; i <= xR; i++){
        if(map->cells[i][cellY]->bomb != nullptr){
            searchInRange(map, i, cellY);
            break;
        }
        if(map->cells[i][cellY]->obstacle != nullptr){
            destroyObstacle(map, i, cellY);
            break;
        }
        killPlayerOnField(map, i, cellY);
    }

    for(int i = cellX; i >= xL; i--){
        if(map->cells[i][cellY]->bomb != nullptr){
            searchInRange(map, i, cellY);
            break;
        }
        if(map->cells[i][cellY]->obstacle != nullptr){
            destroyObstacle(map, i, cellY);
            break;
        }
        killPlayerOnField(map, i, cellY);
    }

    for(int i = cellY; i >= yU; i--){
        if(map->cells[cellX][i]->bomb != nullptr){
            searchInRange(map, cellX, i);
            break;
        }
        if(map->cells[cellX][i]->obstacle != nullptr){
            destroyObstacle(map, cellX, i);
            break;
        }
        killPlayerOnField(map, cellX, i);
    }

    for(int i = cellY; i <= yD; i++){
        if(map->cells[cellX][i]->bomb != nullptr){
            searchInRange(map, cellX, i);
            break;
        }
        if(map->cells[cellX][i]->obstacle != nullptr){
            destroyObstacle(map, cellX, i);
            break;
        }
        killPlayerOnField(map, cellX, i);
    }
}

void destroyObstacle(Map* map, int x, int y){
    if(map->cells[x][y]->obstacle->isDestroyable()){
        pthread_mutex_lock(&map->mutex);
        map->cells[x][y]->obstacle = nullptr;
        pthread_mutex_unlock(&map->mutex);
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
                cout<<map->players.at(i)->lifes<<endl;
                continue;
            }
        }
    }
}