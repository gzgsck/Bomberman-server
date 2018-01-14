//
// Created by grzegorz on 01.12.17.
//

#ifndef PROJECTBOOM_GAMEJUDGE_H
#define PROJECTBOOM_GAMEJUDGE_H


#include "../headers/Map.h"

void manageBombsExplosions(Map* map);
void manageFires(Map* map);
void searchInRange(Map* map, int cellX, int cellY);
void destroyObstacle(Map* map, int x, int y);
void killPlayerOnField(Map* map, int x, int y);
void managePlayers(Map* map, long timediff);


#endif //PROJECTBOOM_GAMEJUDGE_H
