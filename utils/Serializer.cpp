#include "Serializer.h"
#include "../json_spirit/json_spirit_writer.h"
#include "../json_spirit/json_spirit_value.h"
#include "../json_spirit/json_spirit_writer_options.h"
#include "../json_spirit/json_spirit_writer_template.h"
#include "../json_spirit/json_spirit.h"
#include "../headers/Map.h"
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;

string serializeToJson(Map* map){
    json_spirit::Object addr_obj;

    addr_obj.push_back( json_spirit::Pair( "mapSize", map->mapSize ) );

    vector<string> bombs;
    for(int i =0 ; i< map->bombs.size(); i++) {
        json_spirit::Object temp;
        temp.push_back(json_spirit::Pair("bomb", bombSerializer(map->bombs.at(i))));
        string str = write(temp, json_spirit::raw_utf8);
        bombs.push_back(str);
    }

    json_spirit::Value val( bombs.begin(), bombs.end());
    string str1 = write( val, json_spirit::single_line_arrays);
    addr_obj.push_back( json_spirit::Pair( "bombs", str1 ) );

    vector<string> players;
    for(int i = 0 ; i< map->players.size(); i++) {
        json_spirit::Object temp1;
        temp1.push_back(json_spirit::Pair("player", playerSerializer(map->players.at(i))));
        string str = write(temp1, json_spirit::raw_utf8);
        players.push_back(str);
    }

    json_spirit::Value val1( players.begin(), players.end());
    string str2 = write( val1, json_spirit::single_line_arrays);
    addr_obj.push_back( json_spirit::Pair( "players", str2 ) );

    string str = json_spirit::write( addr_obj, json_spirit::pretty_print | json_spirit::raw_utf8  );

    boost::erase_all(str, "\\");

    return str;
}

string bombSerializer(Bomb* bomb){
    json_spirit::Object addr_obj;
    addr_obj.push_back(json_spirit::Pair("power", bomb->power));
    addr_obj.push_back(json_spirit::Pair("durationTime", bomb->durationTime));
    addr_obj.push_back(json_spirit::Pair("position", cellSerializer(bomb->position)));

    return write(addr_obj, json_spirit::raw_utf8);
}

string pointSerializer(Point* point){
    json_spirit::Object addr_obj;
    addr_obj.push_back(json_spirit::Pair("x", point->x));
    addr_obj.push_back(json_spirit::Pair("y", point->y));
    return write(addr_obj, json_spirit::raw_utf8);
}

string cellSerializer(Cell* cell){
    json_spirit::Object addr_obj;
    addr_obj.push_back(json_spirit::Pair("center", pointSerializer(cell->center)));

    return write(addr_obj, json_spirit::none);
}

string playerSerializer(Player* player){
    json_spirit::Object addr_obj;
    addr_obj.push_back(json_spirit::Pair("score", player->score));
    addr_obj.push_back(json_spirit::Pair("name", player->name));
    addr_obj.push_back(json_spirit::Pair("lifes", player->lifes));
    addr_obj.push_back(json_spirit::Pair("isAlive", player->isAlive));
    addr_obj.push_back(json_spirit::Pair("avaliableBombs", player->avaliableBombs));
    return write(addr_obj, json_spirit::none);
}
