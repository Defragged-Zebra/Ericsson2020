//
// Created by woranhun on 2020. 10. 17..
//

#include "country.h"

std::ostream &operator<<(std::ostream &os, const Country &c) {
    os << c.countryID << " " << c.getTotalProductionCapacity() << " " << c.getReserveVaccines();
    return os;
}

//bool Country::isFieldInCountry(size_t ID) {
//    for (auto district:assignedDistricts) {
//        if (district->isFieldInDistrict(ID)) return true;
//    }
//    return false;
//}

bool Country::isNeighbourToVaccinatedField(const Point &p) const {
    if (vaccinatedFields.empty()) {
        return p.isBorder();
    } else {
        for (const auto &selected : p.getNeighbours()) {
            if (vaccinatedFields.find(selected) != vaccinatedFields.end()) return true;
            else if (border.find(selected) != border.end()) return true;
        }
    }
    return false;
}
