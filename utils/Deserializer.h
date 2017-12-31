//
// Created by grzegorz on 04.12.17.
//

#ifndef PROJECTBOOM_DESERIALIZER_H
#define PROJECTBOOM_DESERIALIZER_H

#include <iostream>
#include "../headers/Map.h"

string deserializeName(char message[]);
void deserializeMove(char buffer[], Map* map, Player* player);
void deserializeBomb(char buffer[], Map* map, Player* player);
int deserializeMapId(char message[]);

#endif //PROJECTBOOM_DESERIALIZER_H
