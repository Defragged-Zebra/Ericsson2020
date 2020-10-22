//
// Created by woranhun on 2020. 10. 17..
//

#include "country.h"

std::ostream &operator<<(std::ostream &os, const Country &c) {
    os<<c.countryID;
    return os;
}
