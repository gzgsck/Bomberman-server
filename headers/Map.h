#ifndef PROJECTBOOM_MAP_H
#define PROJECTBOOM_MAP_H
#include <iostream>
#include <vector>
#include "Player.h"
#include "Obstacle.h"
#include "Bomb.h"

using namespace std;

class Player;
class Obstacle;
class Bomb;

class Map {

public:
    vector<Player*> players;
    vector<Obstacle*> obstacles;
    vector<Bomb*> bombs;
    int mapSize;
};


#endif //PROJECTBOOM_MAP_H
