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

bool Country::isNeighbourVaccinatedFields(const Point &p) const {
    {
        if (vaccinatedFields.empty()) {
            return p.getY() == 0 or p.getX() == 0 or p.getY() == Point::getGridHeight() - 1 or
                   p.getX() == Point::getGridWidth() - 1;
        } else {
            auto coordinates = p.getNeighbours();
            for (const auto &selected : coordinates) {
                if (!selected.withinBounds())continue;
                auto it = vaccinatedFields.find(selected);
                if (it != vaccinatedFields.end()) return true;
            }
        }
        return false;
    }
}
