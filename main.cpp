#include <iostream>
#include <fstream>
#include <atomic>
#include <pthread.h>
#include "headers/Map.h"
#include "utils/Serializer.h"
#include "utils/ServerUDP.h"
#include "game-judge/GameJudge.h"
#include "map-generator/MapGenerator.h"


using namespace std;

std::atomic<Map*> map;

void *diffThread(void*){
    while(true) {
        sleep(0.5);
        manageBombsExplosions(map);
    }
}

int main() {
    map = generateMap();

    pthread_t thread;
    pthread_create(&thread, NULL, diffThread, NULL);
    connection(map);
    return 0;
}