//
// Created by grzegorz on 04.12.17.
//

#ifndef PROJECTBOOM_DESERIALIZER_H
#define PROJECTBOOM_DESERIALIZER_H

#include <iostream>
#include "../headers/Map.h"

class Deserializer {
    void deserializeProbeRequest(char tab[], Map* map);
    void deserializeBomb(char buffer[], Map* map, sockaddr_in clientAddr);
};


#endif //PROJECTBOOM_DESERIALIZER_H
