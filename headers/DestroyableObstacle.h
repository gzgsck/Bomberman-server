#ifndef PROJECTBOOM_DESTROYABLEOBSTACLE_H
#define PROJECTBOOM_DESTROYABLEOBSTACLE_H
#include <iostream>
#include "Obstacle.h"

using namespace std;

class DestroyableObstacle: public Obstacle{
public:
    bool isDestroyable();
};


#endif //PROJECTBOOM_DESTROYABLEOBSTACLE_H
