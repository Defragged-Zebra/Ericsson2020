//
// Created by woranhun on 2020. 10. 17..
//

#include "sector.h"

Sector::Sector(std::vector<Field *> fields) {
    this->fields = fields;
}

//pass on reference is better, right? if yes, we should change it
std::vector<Field *> Sector::getFields() const {
    return fields;
}