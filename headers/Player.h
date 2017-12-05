#ifndef PROJECTBOOM_PLAYER_H
#define PROJECTBOOM_PLAYER_H
#include <iostream>
#include <vector>
#include "Point.h"
#include "Bomb.h"

using namespace std;

class Point;
class Bomb;

class Player {

public:
    Point* position;
    int id;
    string name;
    int avaliableBombs;
    int lifes;
    bool isAlive;
    Player();
    vector<Bomb*> bombs;
    int lastResponseTime;

};


#endif //PROJECTBOOM_PLAYER_H
