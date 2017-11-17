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
    bomb->durationTime = 18;
    bomb->power = 1;
    bombs.push_back(bomb);
    Bomb* bomb1 = new Bomb();
    bomb1->durationTime = 12;
    bomb1->power = 1;
    bombs.push_back(bomb1);

    Cell* cell = new Cell();

    Point* point = new Point();
    point->x = 1;
    point->y = 2;
    cell->center = point;
    bomb->position = cell;
    bomb1->position = cell;
    m->bombs = bombs;

    Player* player = new Player();
    player->position = point;
    player->bombs = bombs;
    player->avaliableBombs = 2;
    player->isAlive = true;
    player->lifes = 3;
    player->name = "jan";
    player->score = 8;

    m->players.push_back(player);
    m->players.push_back(player);


    cout<<serializeToJson(m);
    return 0;
}