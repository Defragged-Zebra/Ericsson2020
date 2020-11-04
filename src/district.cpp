//
// Created by woranhun on 2020. 10. 17..
//

#include "district.h"

std::ostream &operator<<(std::ostream &os, const District &d) {
    os << d.districtID;
    return os;
}

//TODO: unit test/review this
void District::updateIsClear() {
    if (clear)
        throw std::runtime_error(
                "district.cpp - You tried to update the clean-ness of a clean field. This shouldn't happen, pls notify Spark");
    bool tmp = true;
    for (int i = 0; i < assignedFields.size(); ++i) {
        tmp &= (*assignedFields[i]).isClear();
    }
    clear = tmp;
}

