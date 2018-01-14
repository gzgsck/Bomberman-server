//
// Created by grzegorz on 16.11.17.
//

#include <netinet/in.h>
#include <chrono>
#include "../headers/Map.h"
#define MAX 500



Map::Map() {
    int i;
    do {
        i = rand() % NAMES_LENGTH;
    } while(NAMES[i].length() == 0);
    
    this->name = NAMES[i];
    NAMES[i] = "";

    this->access = semget(IPC_PRIVATE, 1, IPC_CREAT|0600);
    if (semctl(access, 0, SETVAL, (int)1) == -1) {
      perror("Nadanie wartosci semaforowi Map");
      exit(1);
    }
    this->receive = semget(IPC_PRIVATE, 1, IPC_CREAT|0600);
    if (semctl(receive, 0, SETVAL, (int)0) == -1) {
      perror("Nadanie wartosci semaforowi Map");
      exit(1);
    }

    this->messageReceived = 0;
}


bool Map::checkAllPlayersHaveName() {
    for(int i = 0 ; i < this->players.size(); i++){
        if((this->players.at(i)->name.compare("") == 0 )){
            return false;
        }
    }
    return true;
}

int Map::checkIsOnPlayersList(string name) {
    for (int i = 0 ; i< this->players.size(); i++){
        if(this->players.at(i)->name.compare(name) == 0){
            return i;
        }
    }
    return -1;
}

int Map::addPlayersNameToList(string name) {
    if (checkAllPlayersHaveName()) {
        return -1;
    }
    for(int i = 0; i < this->players.size(); i++){
        if(this->players.at(i)->name.size()<1){
            this->players.at(i)->name = name;
            return i;
        }
    }
}

void Map::setPlayerMove(Player* player, int x, int y) {
    if (!this->canMoveTo(player, player->position->x + x, player->position->y + y)) {
        return;
    }

    Cell* cell = getCellByPosition((player->position->x + x)/MAP_FIELD_SIZE, (player->position->y + y)/MAP_FIELD_SIZE);
    if (cell->fire != nullptr) {
        if (!player->isProtected) {
            player->lifes -= 1;

            if (player->lifes < 1) {
                player->isAlive = false;
            }
            player->setProtection();
        }
    }

    player->position->y += y;
    player->position->x += x;
}

bool Map::canMoveTo(Player* player, int x , int y) {
    Cell* cell = getCellByPosition(x/MAP_FIELD_SIZE, y/MAP_FIELD_SIZE);
    if (cell == nullptr) {
        return false;
    }
    if (player->position->x/MAP_FIELD_SIZE == x/MAP_FIELD_SIZE && player->position->y/MAP_FIELD_SIZE == y/MAP_FIELD_SIZE){
       return true;
    }
    if (cell->obstacle != nullptr || cell->bomb != nullptr){
        return false;
    }
    return true;
}

void Map::setBombPlant(Player* player, int x, int y) {
    Cell* cell = getCellByPosition(x, y);
    if(canPlantBomb(player, cell) == false){
        return;
    }
    Bomb* bomb = new Bomb(player, player->bombPower);
    player->plantBomb(bomb);
    bomb->owner = player;
    cell->bomb = bomb;
}

bool Map::canPlantBomb(Player* player, Cell* cell){
    if(player->avaliableBombs - player->bombs.size() <= 0) {
        cout << "No more bombs" << endl;
        return false;
    }
    if(cell->bomb != nullptr){
        cout << "Already bomb" << endl;
        return false;
    }
    if(cell->obstacle != nullptr){
        return false;
    }
    return true;
}

Cell* Map::getCellByPosition(int x, int y) {
    if (x < 0 || y < 0 || x > MAP_SIZE - 1 || y > MAP_SIZE - 1) return nullptr;
    return this-> cells[y][x];
}

int NAMES_LENGTH = 89;
string NAMES[] = {
    "Greenspring",
    "Wellspring",
    "Hedgebourne",
    "Fairdragon",
    "Northnesse",
    "Spellborough",
    "Silversage",
    "Lorwald",
    "Icesage",
    "Icemarble",
    "Oldgold",
    "Blueviolet",
    "Fayhaven",
    "Starryhall",
    "Wellness",
    "Stonespell",
    "Shadowwyvern",
    "Flowerdell",
    "Merribush",
    "Fogmere",
    "Riverhedge",
    "Roseby",
    "Grasskeep",
    "Bypond",
    "Eastmallow",
    "Lochbarrow",
    "Southston",
    "Wyvernholt",
    "Dorcliff",
    "Estermage",
    "Silverglass",
    "Fairshore",
    "Pinedell",
    "Mapleford",
    "Castleedge",
    "Wheatcliff",
    "Esterloch",
    "Brightdale",
    "Starryshore",
    "Fieldhollow",
    "Westness",
    "Marbleacre",
    "Redholt",
    "Marblefair",
    "Lightwind",
    "Greenmoor",
    "Springhedge",
    "Wyvernston",
    "Elfbridge",
    "Shoreley",
    "Westercastle",
    "Eriwald",
    "Woodelf",
    "Newmere",
    "Lightlake",
    "Newhall",
    "Applecoast",
    "Butterwall",
    "Foghill",
    "Redgrass",
    "Butterriver",
    "Highmere",
    "Griffinham",
    "Woodloch",
    "Wolfbourne",
    "Roseville",
    "Oldbeach",
    "Bymill",
    "Silvershore",
    "Marblelake",
    "Jannesse",
    "Waterbridge",
    "Aelholt",
    "Whiteby",
    "Springpond",
    "Clearfair",
    "Greycrystal",
    "Westerburn",
    "Morton",
    "Icewynne",
    "Merrowshore",
    "Barrowcastle",
    "Normill",
    "Aelwick",
    "Linbridge",
    "Eastbell",
    "Strongmoor",
    "Griffinshore",
    "Dorgate"
};

