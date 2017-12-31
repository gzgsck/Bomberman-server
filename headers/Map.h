#ifndef PROJECTBOOM_MAP_H
#define PROJECTBOOM_MAP_H
#include <iostream>
#include <vector>
#include "Player.h"
#include "Obstacle.h"
#include "Bomb.h"
#include "Cell.h"
#include "Fire.h"
#include "../utils/Configuration.h"

using namespace std;

class Player;
class Obstacle;
class Bomb;
class Fire;

class Map {

public:
    int id;
    string status;
    vector<Player*> players;
    Cell* cells[MAP_SIZE][MAP_SIZE];
    bool checkAllPlayersHaveName();
    int checkIsOnPlayersList(string name);
    int addPlayersNameToList(string name);
    bool checkPlayerRequests(string name);
    void setPlayerMove(Player* player, int x, int y);
    void setBombPlant(Player* player, int x, int y);
    Cell* getCellByPosition(int x, int y);
    bool canPlantBomb(Player* player, Cell* cell);
    bool canMoveTo(Player* player, int x , int y);
};


#endif //PROJECTBOOM_MAP_H
