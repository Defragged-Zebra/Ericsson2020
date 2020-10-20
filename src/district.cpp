//
// Created by woranhun on 2020. 10. 17..
//

#include "district.h"

District::District(int sectorID, std::vector<Field *> fields, bool clear = false) {
    this->sectorID = sectorID;
    this->fields = fields;
    this->clear = clear;
}

//pass on reference is better, right? if yes, we should change it
std::vector<Field *> District::getFields() const {
    return fields;
}

int District::getID() const {
    return this->sectorID;
}

bool District::isClear() {
    return this->clear
}