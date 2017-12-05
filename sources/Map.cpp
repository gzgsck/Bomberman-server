//
// Created by grzegorz on 16.11.17.
//

#include <netinet/in.h>
#include "../headers/Map.h"


bool Map::checkAllPlayersHaveName(){
    for(int i = 0 ; i < this->players.size(); i++){
        if(!(this->players.at(i)->name.size()>0)){
            return false;
        }
    }
    return true;
}

bool Map::checkIsOnPlayersList(string name){
    for(int i = 0 ; i< this->players.size(); i++){
        if(this->players.at(i)->name.compare(name) == 0){
            return true;
        }
        return false;
    }
}

bool Map::addPlayersNameToList(string name, sockaddr_in* sock){
    if(checkAllPlayersHaveName()){
        return false;
    }
    for(int i = 0; i < this->players.size(); i++){
        if(this->players.at(i)->name.size()<1){
            this->players.at(i)->name = name;
            this->players.at(i)->socket = sock;
            return true;
        }
    }
}