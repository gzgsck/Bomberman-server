#include <iostream>
#include <fstream>
#include "headers/Map.h"
#include "utils/Serializer.h"
#include "utils/ServerUDP.h"
#include "map-generator/MapGenerator.h"


using namespace std;


int main() {
    Map* m = generateMap();

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