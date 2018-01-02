#include "../headers/LookupTable.h"


LookupRecord::LookupRecord(char request[], int requestLength,  char response[], int responseLength, sockaddr_in* address) {
    lastAccess = chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
        memset(&this->address, 0, sizeof(struct sockaddr_in));
    this->address.sin_addr = address->sin_addr;
    this->address.sin_addr.s_addr = address->sin_addr.s_addr;
    this->address.sin_family = address->sin_family;
    this->address.sin_port = address->sin_port;

    strncpy(this->request, request, requestLength);
    strncpy(this->response, response, responseLength);
    this->requestLength = requestLength;
    this->responseLength = responseLength;
}

bool LookupRecord::isSame(char retryRequest[], int retryRequestLength, sockaddr_in* address) {
    if (retryRequestLength - 2 != this->requestLength) return false;

    if (retryRequest[2] != this->request[0] ||
        retryRequest[3] != this->request[1]
    ) return false; 
    

    if (address->sin_addr.s_addr != this->address.sin_addr.s_addr ||
        address->sin_port != this->address.sin_port) return false;

    printf("%s\n",retryRequest+4);
    printf("%s\n", this->request + 2);

    return strncmp(retryRequest + 4, this->request + 2, this->requestLength - 2) == 0;
}

bool LookupRecord::isOutdated() {
    int now = chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    return lastAccess + 1000 < now;
}

void LookupRecord::copyResponse(char destination[]) {
    lastAccess = chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    strncpy(destination, this->response, this->responseLength);
}


void LookupTable::add(char request[], int requestLength,  char response[], int responseLength, sockaddr_in* address) {
    records.push_back(new LookupRecord(request, requestLength, response, responseLength, address));
}

void LookupTable::search(char retryRequest[], int retryRequestLength, char response[], sockaddr_in* address) {
    for (LookupRecord* record: records) {
        if (record->isSame(retryRequest, retryRequestLength, address)) {
            cout << "Found record: " << record->responseLength << endl;
            record->copyResponse(response);
            response[record->responseLength] = '\0';
            return;
        }
    }
    response[0] = '\0';
}

void LookupTable::removeOutdated() {
    vector<LookupRecord*> newRecords;
    auto it = std::copy_if(records.begin(), records.end(), std::back_inserter(newRecords), [](LookupRecord* record) {
        return !record->isOutdated();
    });
    this->records = newRecords;

}