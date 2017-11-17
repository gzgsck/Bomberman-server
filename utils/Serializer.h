//
// Created by grzegorz on 17.11.17.
//

#ifndef PROJECTBOOM_SERIALIZER_H
#define PROJECTBOOM_SERIALIZER_H

#include <iostream>
#include "../headers/Map.h"

using namespace std;

string serializeToJson(Map* map);
string bombSerializer(Bomb* bomb);

#endif //PROJECTBOOM_SERIALIZER_H
