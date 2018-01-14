#include "../headers/Connection.h"

Connection::Connection(sockaddr_in* address) {
    memset(&this->address, 0, sizeof(struct sockaddr_in));
    this->address.sin_addr = address->sin_addr;
    this->address.sin_addr.s_addr = address->sin_addr.s_addr;
    this->address.sin_family = address->sin_family;
    this->address.sin_port = address->sin_port;
    lastReceiveTime = chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    map = nullptr;
    player = nullptr;
}

void Connection::setNowLastReceive() {
    lastReceiveTime = chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}