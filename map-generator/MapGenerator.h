//
// Created by grzegorz on 02.12.17.
//

#ifndef PROJECTBOOM_MAPGENERATOR_H
#define PROJECTBOOM_MAPGENERATOR_H

#include <iostream>
#include <vector>
#include "../headers/Map.h"

using namespace std;

Map* generateMap();
Map* generateCells(Map* map);
Map* generateObstacles(Map* map);
Map* generatePlayers(Map* map);




#endif //PROJECTBOOM_MAPGENERATOR_H
