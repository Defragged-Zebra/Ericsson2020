//
// Created by woranhun on 2020. 10. 17..
//

#include "country.h"

std::ostream &operator<<(std::ostream &os, const Country &c) {
    os << "Country ID: "<<c.countryID << std::endl<<"TPC: " << c.getTotalProductionCapacity() <<std::endl<< "RV: " << c.getReserveVaccines()<<std::endl;
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
            for (const auto &vd:vaccinatedFields) {
                if (vd.second.find(selected) != vd.second.end()) return true;
            }
            if (border.find(selected) != border.end()) return true;
        }
    }
    return false;
}
bool Country::isNeighbourToWannabeVaccinatedField(const Point &p) const {
    //if (wannabeVaccinatedFields.empty()) {
    //    return false;
    //} else {
        for (const auto &selected : p.getNeighbours()) {
            for (const auto &vd:wannabeVaccinatedFields) {
                if (vd.second.find(selected) != vd.second.end()) return true;
            }
            if (wannabeBorder.find(selected) != wannabeBorder.end()) return true;
        }
    //}
    return false;
}