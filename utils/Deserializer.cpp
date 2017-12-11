//
// Created by grzegorz on 04.12.17.
//
#include <string>
#include <cstring>
#include "Deserializer.h"
#include "ServerUDP.h"

string deserializeProbeRequest(char tab[] ){
    int size = (int)tab[2] - 48;
    char returnedValue[size];
    for(int i = 0; i < size; i++){
        returnedValue[i] = tab[i+4];
    }
    string name;
    name = std::string(returnedValue, size);
    return name;
}

void deserializeMove(char buffer[], Map* map, sockaddr_in clientAddr) {
    string bombs;
    bombs = std::string(buffer, 10);
    char tab[4];
    char tab2[4];
    strncpy(tab, buffer + 2, 4);
    int *x = (int *) tab;
    strncpy(tab2, buffer + 6, 4);
    int *y = (int *) tab2;
    map->setPlayerMove(&clientAddr, *x, *y);
}

    //todo refactor
void deserializeBomb(char buffer[], Map* map, sockaddr_in clientAddr){

    string bombs;
    bombs = std::string(buffer, 10);

    char tab[4];
    char tab2[4];
    strncpy(tab, buffer + 2, 4);
    int x = *((int*)tab);
    strncpy(tab2, buffer + 6, 4);
    int y =*((int*)tab2);
    map->setBombPlant(&clientAddr, x, y);

}


