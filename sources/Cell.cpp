//
// Created by grzegorz on 16.11.17.
//

#include "../headers/Cell.h"

Cell::Cell(Point* point) {
    center = point;
    fire = nullptr;
    bomb = nullptr;
    obstacle = nullptr;
}