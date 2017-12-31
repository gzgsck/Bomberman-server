#include <iostream>
#include <fstream>
#include <atomic>
#include <pthread.h>
#include "headers/Map.h"
#include "utils/Serializer.h"
#include "utils/ServerUDP.h"


using namespace std;

int main() {
    startServer();
    return 0;
}