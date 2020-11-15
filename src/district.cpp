//
// Created by woranhun on 2020. 10. 17..
//


#include "district.h"

std::ostream &operator<<(std::ostream &os, const District &d) {
    os << d.districtID;
    return os;
}

//returns which country cleared it
bool District::updateIsClear() {
    if (clear)
        throw std::runtime_error(
                "district.cpp - You tried to update the clean-ness of a clean field. This shouldn't happen, pls notify Spark");
    bool tmp = true;
    for (auto &assignedField : assignedFields) {
        tmp &= (assignedField)->isClear();
    }
    clear = tmp;
    return clear;
}

bool District::isFieldInDistrict(size_t fieldID) {
    //another lambda here
    return std::any_of(assignedFields.begin(), assignedFields.end(),
                [fieldID](auto currentField) { return currentField->getFieldID() == fieldID; });
    /*
    for (auto field:assignedFields) {
        if (field->getFieldID() == fieldID) return true;
    }
    return false;
     */
}

void District::clearAssignedFields() {
    assignedFields.clear();
}

