#ifndef PROJECTBOOM_FIRE_H
#define PROJECTBOOM_FIRE_H
#include <iostream>
#include "Cell.h"
#include "Player.h"

using namespace std;

class Cell;
class Player;

class Fire {

public:
    int durationTime;
    long timestamp;
    Fire(int duration);
};


#endif //PROJECTBOOM_FIRE_H
