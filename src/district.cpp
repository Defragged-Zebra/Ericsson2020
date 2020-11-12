//
// Created by woranhun on 2020. 10. 17..
//


#include "district.h"

std::ostream &operator<<(std::ostream &os, const District &d) {
    os << d.districtID;
    return os;
}

void District::updateIsClear() {
    if (clear)
        throw std::runtime_error(
                "district.cpp - You tried to update the clean-ness of a clean field. This shouldn't happen, pls notify Spark");
    bool tmp = true;
    for (auto &assignedField : assignedFields) {
        tmp &= (*assignedField).isClear();
    }
    clear = tmp;
}

//TODO: test this
void District::simplifyNeighbourDistrictVector() {
    // sort followed by unique, to remove all duplicates
    std::sort(neighbourDistricts.begin(), neighbourDistricts.end());
    std::unique(neighbourDistricts.begin(), neighbourDistricts.end());
}

