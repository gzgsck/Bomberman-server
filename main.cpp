#include <iostream>
#include <fstream>
#include <atomic>
#include "headers/Map.h"
#include "utils/Serializer.h"
#include "utils/ServerUDP.h"
#include "map-generator/MapGenerator.h"


using namespace std;

std::atomic<Map*> map;

int main() {
    map = generateMap();
    connection(map);
    return 0;
}