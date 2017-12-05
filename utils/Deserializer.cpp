//
// Created by grzegorz on 04.12.17.
//
#include <string>
#include "Deserializer.h"

string deserializeProbeRequest(char tab[] ){
    //todo zaimplementowac char na stirng
    int size = (int)tab[2] - 48;
    cout<<size<<endl;
    char returnedValue[size];
    for(int i = 0; i < size; i++){
        returnedValue[i] = tab[i+4];
    }
    string name;
    name = std::string(returnedValue, size);
    cout<<name<<endl;
    return name;
}