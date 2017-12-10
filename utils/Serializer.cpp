#include "Serializer.h"

using namespace std;


string serializeObstacles(Map* map){
    string obstacles = "o:";
    obstacles.append(to_string(MAP_SIZE * MAP_SIZE));
    obstacles.append("|");
    for(int i = 0; i < MAP_SIZE ; i++) {
        for (int k = 0; k < MAP_SIZE; k++) {
            if (map->cells[i][k]->obstacle == nullptr) {
                obstacles.append("0");
                continue;
            }
            if (map->cells[i][k]->obstacle->isDestroyable()) {
                obstacles.append("1");
                continue;
            } else {
                obstacles.append("2");
                continue;
            }
        }
    }
    obstacles.append("+++");
    return obstacles;
}

string serializePlayers(Map* map){
    string players = "p:";
    players.append(to_string(map->players.size()));
    players.append("|");
    for(int i = 0; i < map->players.size(); i++){
        Player* p = map->players.at(i);
        players.append(to_string(p->id));
        players.append(",");
        players.append(to_string(p->lifes));
        players.append(",");
        players.append(to_string(p->isAlive));
        players.append(",");
        players.append(to_string(p->avaliableBombs));
        players.append(",");
        players.append(to_string(p->position->x));
        players.append(",");
        players.append(to_string(p->position->y));
        if (i != map->players.size() - 1) {
            players.append("|");
        }

    }
    return players;
}

string serializeBombs(Map* map){
    int bombsQuantity = 0;
    string bombs = "";
    for(int i = 0; i < MAP_SIZE ; i++) {
        for (int k = 0; k < MAP_SIZE; k++) {
            if (map->cells[i][k]->bomb != nullptr) {
                bombsQuantity++;
                bombs.append(to_string(i*MAP_SIZE + k));
                bombs.append(",");
                bombs.append(to_string(map->cells[i][k]->bomb->power));
                bombs.append(",");
                bombs.append(to_string(map->cells[i][k]->bomb->durationTime));
                bombs.append(",");
                bombs.append(to_string(map->cells[i][k]->bomb->timestamp));
                bombs.append("|");
            }
        }
    }
    string returnedValue = "";
    returnedValue.append("b:");
    returnedValue.append(to_string(bombsQuantity));
    returnedValue.append("|");
    returnedValue.append(bombs);
    returnedValue.append("+++");
    return returnedValue;

}
string serializeToTableOfPlayers(Map* map, int status, Player* player) {
    string players = "pl:";
    players.append(to_string(status));
    players.append(",");
    players.append(to_string(player->id));

    for(int i = 0; i< map->players.size(); i++){
        players.append("|");
        Player* p = map->players.at(i);
        players.append(to_string(p->id));
        players.append(",");
        players.append(p->name);

    }
    return players;
}

// todo deserialize
// move x,y
// bomba x,y
// ping
// tablica gracz - struktura z ip, czas aktywnosci
// w 1 forze