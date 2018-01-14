//
// Created by grzegorz on 04.12.17.
//
#include <string>
#include <cstring>
#include "Deserializer.h"
#include "ServerUDP.h"

string deserializeName(char message[]) {
    int size = (int)message[2] - 48;
    char returnedValue[size];
    strncpy(returnedValue, message+4, size);
    string name = std::string(returnedValue, size);
    return name;
}

int deserializeMapId(char message[]) {
    int size = ((int)message[2] - 48) + 4;
    char intBites[4];
    strncpy(intBites, message + size, 4);
    return *((int*) intBites);
}

void deserializeMove(char buffer[], Map* map, Player* player) {
    player->direction = buffer[2];
}

void deserializeBomb(char buffer[], Map* map, Player* player) {
    char tab[4];
    char tab2[4];
    strncpy(tab, buffer + 2, 4);
    int x = *((int*)tab);
    strncpy(tab2, buffer + 6, 4);
    int y =*((int*)tab2);
    map->setBombPlant(player, x, y);

}


