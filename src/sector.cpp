//
// Created by woranhun on 2020. 10. 17..
//

#include "sector.h"

Sector::Sector(int sectorID, std::vector<Field *> fields, bool clear = false) {
    this->sectorID = sectorID;
    this->fields = fields;
    this->clear = clear;
}

//pass on reference is better, right? if yes, we should change it
std::vector<Field *> Sector::getFields() const {
    return fields;
}

int Sector::getID() const {
    return this->sectorID;
}

bool Sector::isClear() {
    return this->clear
}