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
    json_spirit::Object temp;
    for(int i =0 ; i< map->bombs.size(); i++) {
        temp.push_back(json_spirit::Pair("bomb", bombSerializer(map->bombs.at(0))));
        string str = write(temp, json_spirit::raw_utf8);
        bombs.push_back(str);
    }

    json_spirit::Value val( bombs.begin(), bombs.end());
    string str1 = write( val, json_spirit::single_line_arrays);
    addr_obj.push_back( json_spirit::Pair( "bombs", str1 ) );
    string str = json_spirit::write( addr_obj, json_spirit::pretty_print | json_spirit::raw_utf8  );

    boost::erase_all(str, "\\");

    return str;
}

string bombSerializer(Bomb* bomb){
    json_spirit::Object addr_obj;
    addr_obj.push_back(json_spirit::Pair("power", bomb->power));
    addr_obj.push_back(json_spirit::Pair("durationTime", bomb->durationTime));

    return write(addr_obj, json_spirit::none);
}
