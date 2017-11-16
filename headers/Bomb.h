#ifndef PROJECTBOOM_BOMB_H
#define PROJECTBOOM_BOMB_H
#include <iostream>
#include "Cell.h"
#include "Player.h"

using namespace std;

class Cell;
class Player;

class Bomb {

private:
    Cell* position;
    Player* owner;
    int power;
    int durationTime;

};


#endif //PROJECTBOOM_BOMB_H
