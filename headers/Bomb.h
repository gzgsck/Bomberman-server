#ifndef PROJECTBOOM_BOMB_H
#define PROJECTBOOM_BOMB_H
#include <iostream>
#include "Cell.h"
#include "Player.h"

using namespace std;

class Cell;
class Player;

class Bomb {

public:
    Player* owner;
    int power;
    int durationTime;
    long timestamp;
    Bomb(Player* owner, int power);
};


#endif //PROJECTBOOM_BOMB_H
