#include <iostream>
#include <fstream>
#include "headers/Map.h"
#include "utils/Serializer.h"


using namespace std;

int main() {
    Map* m = new Map();
    m->mapSize = 10;
    vector<Bomb*> bombs;
    Bomb* bomb = new Bomb();
    bomb->durationTime = 13;
    bomb->power = 1;
    bombs.push_back(bomb);
    Bomb* bomb1 = new Bomb();
    bomb->durationTime = 12;
    bombs.push_back(bomb1);
    m->bombs = bombs;


    cout<<serializeToJson(m);
    return 0;
}