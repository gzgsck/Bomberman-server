//
// Created by grzegorz on 16.11.17.
//

#include <cstring>
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
}

void Player::setSocket(sockaddr_in *sock) {
    memset(&this->socket, 0, sizeof(struct sockaddr_in));
    this->socket.sin_addr = sock->sin_addr;
    this->socket.sin_addr.s_addr = sock->sin_addr.s_addr;
    //this->socket.sin_zero = sock->sin_zero;
    this->socket.sin_family = sock->sin_family;
    this->socket.sin_port = sock->sin_port;
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