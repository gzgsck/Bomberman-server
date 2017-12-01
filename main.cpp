#include <iostream>
#include <fstream>
#include "headers/Map.h"
#include "utils/Serializer.h"
#include "utils/ServerUDP.h"


using namespace std;


int main() {
    Map* m = new Map();
    m->mapSize = 50;

    for(int i =0 ; i< m->mapSize ; i++) {
        Cell *cell = new Cell();

        if(i % 3 == 0) {
            cell->obstacle =  new DestroyableObstacle();
            cell->bomb = new Bomb();
            cell->bomb->durationTime = 100;
            cell->bomb->power = 8;
        }
        if(i % 3 == 1) {cell->obstacle = new UndestroyableObstacle();}
        m->cells.push_back(cell);
    }

    for(int i = 0 ; i< 5; i++){
        Player* p = new Player();
        p->position->x = 12;
        p->position->y = 55;
        p->name = "okok";
        p->avaliableBombs = 3;
        p->isAlive = true;
        p->lifes = 12;
        m->players.push_back(p);
    }



    connection(m);
    return 0;
}