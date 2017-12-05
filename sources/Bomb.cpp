//
// Created by grzegorz on 16.11.17.
//

#include <chrono>
#include "../headers/Bomb.h"

Bomb::Bomb(Player* player, int aPower) {
    owner = player;
    power = aPower;
    timestamp = chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}