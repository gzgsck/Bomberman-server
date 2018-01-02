#include <iostream>
#include <fstream>
#include <atomic>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "headers/Map.h"
#include "utils/Serializer.h"
#include "utils/ServerUDP.h"

using namespace std;

int main() {
    srand (time(NULL));
    startServer();
    return 0;
}