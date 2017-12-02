//
// Created by grzegorz on 18.11.17.
//

#include "ServerUDP.h"
#include "Serializer.h"
#include "Configuration.h"


ssize_t sendto(int i, char string[50], int i1, int i2);

int connection(Map* map )
{
    int nSocket;
    int nBind, nListen;
    int nFoo = 1;
    socklen_t nTmp;
    struct sockaddr_in stAddr, stClientAddr;


    /* address structure */
    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(SERVER_PORT);

    /* create a socket */
    nSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (nSocket < 0)
    {
        fprintf(stderr, "Can't create a socket.\n");
        exit(1);
    }

    setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));
    /* bind a name to a socket */
    nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if (nBind < 0)
    {
        fprintf(stderr," Can't bind a name to a socket.\n");
        exit(1);
    }


    while(1)
    {
        nTmp = sizeof(struct sockaddr);
        int n = 500;
        char buffer[n];
        recvfrom(nSocket, buffer, n, 0,(struct sockaddr*)&stClientAddr, &nTmp);




        string s = serializeObstacles(map);
        strcpy(buffer, s.c_str());
        sendto(nSocket, buffer, n, 0,(struct sockaddr*)&stClientAddr, sizeof(stClientAddr));

        string o = serializeBombs(map);
        strcpy(buffer, o.c_str());
        sendto(nSocket, buffer, n, 0,(struct sockaddr*)&stClientAddr, sizeof(stClientAddr));

        string p = serializePlayers(map);
        strcpy(buffer, p.c_str());
        sendto(nSocket, buffer, n, 0,(struct sockaddr*)&stClientAddr, sizeof(stClientAddr));
    }

}



