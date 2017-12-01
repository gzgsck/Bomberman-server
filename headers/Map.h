#ifndef PROJECTBOOM_MAP_H
#define PROJECTBOOM_MAP_H
#include <iostream>
#include <vector>
#include "Player.h"
#include "Obstacle.h"
#include "Bomb.h"
#include "Cell.h"

using namespace std;

class Player;
class Obstacle;
class Bomb;

class Map {

public:
    vector<Player*> players;
    vector<Cell*> cells;
    int mapSize;
};


#endif //PROJECTBOOM_MAP_H
