//
// Created by woranhun on 2020. 10. 17..
//

#include "district.h"

District::District(int sectorID, std::vector<int> fields, bool clear = false) {
    this->districtID = sectorID;
    this->assignedFieldIDs = fields;
    this->clear = clear;
}

//pass on reference is better, right? if yes, we should change it
std::vector<int> District::getAssignedFields() const {
    return assignedFieldIDs;
}

int District::getDistrictID() const {
    return this->districtID;
}

bool District::isClear () const {
    return this->clear
}