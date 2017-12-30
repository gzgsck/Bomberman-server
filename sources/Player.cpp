//
// Created by grzegorz on 16.11.17.
//

#include <cstring>
#include <chrono>
#include "../headers/Player.h"
#include "../utils/Configuration.h"

Player::Player() {
    this->avaliableBombs = PLAYERS_START_BOMBS_QUANTITY;
    this->lifes = PLAYERS_START_LIFES_QUANTITY;
    this->isAlive = true;
    this->position = new Point();
    this->isActive = true;
    this->resetPosition = false;
    this->bombPower = PLAYERS_START_BOMB_POWER;
    this->isProtected = false;
    this->protectionStartTime = 0;
}

void Player::plantBomb(Bomb* bomb) {
    this->bombs.push_back(bomb);
}

bool Player::isPlayerOnField(int row, int col) {
    int playerX = this->position->x / MAP_FIELD_SIZE;
    int playerY = this->position->y / MAP_FIELD_SIZE;    
    return playerX == col && playerY == row;
}

void Player::removeBomb(Bomb* bomb){
    int i;
    for(i = 0; i < bombs.size(); i++){
        if(bombs.at(i)->timestamp == bomb->timestamp){
            break;
        }
    }
    this->bombs.erase(bombs.begin() + i);
}

void Player::checkProtection(){
    if(isProtected == false){
        return;
    }
    else{
        if(this->protectionStartTime + PROTECTION_DURATION < chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()){
            this->isProtected = false;
        }
    }
}

void Player::setProtection() {
    this->protectionStartTime = chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    this->isProtected = true;
}