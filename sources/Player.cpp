//
// Created by grzegorz on 16.11.17.
//

#include "../headers/Player.h"
#include "../utils/Configuration.h"

Player::Player() {
    this->avaliableBombs = PLAYERS_START_BOMBS_QUANTITY;
    this->lifes = PLAYERS_START_LIFES_QUANTITY;
    this->isAlive = true;
    this->position = new Point();
    this->isActive = true;
}