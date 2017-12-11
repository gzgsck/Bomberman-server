#ifndef PROJECTBOOM_CELL_H
#define PROJECTBOOM_CELL_H
#include <iostream>
#include "Point.h"
#include "Obstacle.h"
#include "Bomb.h"

using namespace std;

class Point;
class Obstacle;
class Bomb;

class Cell {

public:
    Point* center;
    Obstacle* obstacle;
    Bomb* bomb;
    Cell(Point* point);
};


#endif //PROJECTBOOM_CELL_H
