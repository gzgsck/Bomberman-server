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
    map = generatePlayers(map);
    return map;
}

Map* generateObstacles(Map* map){
    for(int i = 0 ; i < MAP_SIZE; i++ ){
        for(int k = 0 ; k < MAP_SIZE; k++){
            int value = rand()%5;
            if(value == 0 || value == 1) {
                map->cells[i][k]->obstacle = new DestroyableObstacle();
            }
            if(value == 2) {
                map->cells[i][k]->obstacle = new UndestroyableObstacle();
            }
        }
    }

    return map;
}

Map* generateCells(Map* map){
    for(int i = 0 ; i < MAP_SIZE; i++ ){
        for(int k = 0 ; k < MAP_SIZE; k++){
            map->cells[i][k] = new Cell(new Point(i*MAP_FIELD_SIZE + MAP_FIELD_SIZE/2,k*MAP_FIELD_SIZE +  MAP_FIELD_SIZE/2));
        }
    }
    return map;
}

Map* generatePlayers(Map* map){
    for(int i = 0 ; i < PLAYERS_QUANTITY ; i++){
        Player* player = new Player();
        player->position->x = map->cells[(i%2)*(MAP_SIZE-1)][((i/2)%2)*(MAP_SIZE-1)]->center->x;
        player->position->y = map->cells[(i%2)*(MAP_SIZE-1)][((i/2)%2)*(MAP_SIZE-1)]->center->y;
        map->players.push_back(player);
        player->id = i + 1;
    }

    return map;
}