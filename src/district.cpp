//
// Created by woranhun on 2020. 10. 17..
//

#include "district.h"

std::ostream &operator<<(std::ostream &os, const District &d) {
    os << d.districtID;
    return os;
}

