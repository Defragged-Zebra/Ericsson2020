//
// Created by woranhun on 2020. 10. 17..
//

#include "country.h"

std::ostream &operator<<(std::ostream &os, const Country &c) {
    os << c.countryID << " " << c.getTotalProductionCapacity() << " " << c.getReserveVaccines();
    return os;
}

bool Country::isFieldInCountry(size_t ID) {
    for (auto district:assignedDistricts) {
        if (district->isFieldInDistrict(ID)) return true;
    }
    return false;
}
