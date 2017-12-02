//
// Created by grzegorz on 02.12.17.
//

#include "MapGenerator.h"
#include "../utils/Configuration.h"
#include "../headers/DestroyableObstacle.h"
#include "../headers/UndestroyableObstacle.h"

Map* generateMap(){
    Map* map = new Map();
    map = generateCells(map);
    map = generateObstacles(map);











    return map;
}

//todo
Map* generateObstacles(Map* map){
    for(int i = 0 ; i < MAP_SIZE; i++ ){
        for(int k = 0 ; k < MAP_SIZE; k++){
            int value = rand()%3;
            if(value == 0) {
                map->cells[i][k]->obstacle = new DestroyableObstacle();
            }
            if(value == 1) {
                map->cells[i][k]->obstacle = new UndestroyableObstacle();
            }
        }
    }

    return map;
}

Map* generateCells(Map* map){
    for(int i = 0 ; i < MAP_SIZE; i++ ){
        for(int k = 0 ; k < MAP_SIZE; k++){
            map->cells[i][k] = new Cell();
        }
    }
    return map;
}