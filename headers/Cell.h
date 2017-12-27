#ifndef PROJECTBOOM_CELL_H
#define PROJECTBOOM_CELL_H
#include <iostream>
#include "Point.h"
#include "Obstacle.h"
#include "Bomb.h"
#include "Fire.h"

using namespace std;

class Point;
class Obstacle;
class Bomb;
class Fire;

class Cell {

public:
    Point* center;
    Obstacle* obstacle;
    Bomb* bomb;
    Fire* fire;
    Cell(Point* point);
};


#endif //PROJECTBOOM_CELL_H
