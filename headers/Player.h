#ifndef PROJECTBOOM_PLAYER_H
#define PROJECTBOOM_PLAYER_H
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include "Point.h"
#include "Bomb.h"
#include "Fire.h"

using namespace std;

class Point;
class Bomb;
class Fire;
class Connection;

class Player {
public:
    void plantBomb(Bomb* bomb);
    bool isPlayerOnField(int x, int y);
    void removeBomb(Bomb* bomb);
    void checkProtection();
    void setProtection();
    void setConnection(Connection *connection);
    Point* position;
    Connection* connection;
    int id;
    string name = "";
    int avaliableBombs;
    int lifes;
    bool isAlive;
    bool isActive;
    bool resetPosition;
    long protectionStartTime;
    bool isProtected;
    Player();

    vector<Bomb*> bombs;
    int bombPower;
};


#endif //PROJECTBOOM_PLAYER_H
