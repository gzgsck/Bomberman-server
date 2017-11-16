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

private:
    Point* position;
    int score;
    string name;
    int avaliableBombs;
    vector<Bomb*> bombs;
    int lifes;
    bool isAlive;

};


#endif //PROJECTBOOM_PLAYER_H
