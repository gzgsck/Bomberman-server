#ifndef PROJECTBOOM_OBSTACLE_H
#define PROJECTBOOM_OBSTACLE_H
#include <iostream>
#include "Cell.h"

using namespace std;

class Obstacle {
public:
    Cell* position;
    virtual bool isDestroyable() = 0;
};


#endif //PROJECTBOOM_OBSTACLE_H
