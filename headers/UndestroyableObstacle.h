#ifndef PROJECTBOOM_UNDESTROYABLEOBSTACLE_H
#define PROJECTBOOM_UNDESTROYABLEOBSTACLE_H
#include <iostream>
#include "Obstacle.h"

using namespace std;

class UndestroyableObstacle: public Obstacle {
public:
    bool isDestroyable();
};


#endif //PROJECTBOOM_UNDESTROYABLEOBSTACLE_H
