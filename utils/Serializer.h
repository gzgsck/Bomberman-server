//
// Created by grzegorz on 17.11.17.
//

#ifndef PROJECTBOOM_SERIALIZER_H
#define PROJECTBOOM_SERIALIZER_H

#include <iostream>
#include "../headers/Map.h"
#include "../headers/Player.h"
#include "../headers/DestroyableObstacle.h"
#include "../headers/UndestroyableObstacle.h"

using namespace std;

string serializeObstacles(Map* map);
string serializeBombs(Map* map);
string serializePlayers(Map* map);
string serializeToTableOfPlayers(Map* map, int status, Player* player);

#endif //PROJECTBOOM_SERIALIZER_H
