#ifndef PROJECTBOOM_LOOKUP_H
#define PROJECTBOOM_LOOKUP_H
#include <vector>
#include <chrono>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <iostream>

using namespace std;


class LookupRecord {
    public:
        int lastAccess;
        char request[500];
        int requestLength;
        char response[500];
        int responseLength;
        sockaddr_in address;

    
    LookupRecord(char request[], int requestLength,  char response[], int responseLength, sockaddr_in* address);
    bool isSame(char retryRequest[], int retryRequestLength, sockaddr_in* address);
    bool isOutdated();
    void copyResponse(char destination[]);
};

class LookupTable {
    public:
    vector<LookupRecord*> records = {};
    void add(char request[], int requestLength,  char response[], int responseLength, sockaddr_in* address);
    void search(char retryRequest[], int retryRequestLength, char response[], sockaddr_in* address);
    void removeOutdated();
};

#endif