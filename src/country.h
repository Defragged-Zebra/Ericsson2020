//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_COUNTRY_H
#define VIRUS_COUNTRY_H


#include <vector>
#include <iostream>

class Country {
    size_t countryID{};
    int totalProductionCapacity{};
    int reserveVaccines{};
    std::vector<size_t> assignedDistrictIDs;
public:
    Country() {
        assignedDistrictIDs = std::vector<size_t>();
    }

    Country(const Country &c) {
        *this = c;
    }

    Country &operator=(const Country &c) {
        if (this != &c) {
            countryID = c.countryID;
            totalProductionCapacity = c.totalProductionCapacity;
            reserveVaccines = c.reserveVaccines;
            assignedDistrictIDs = c.assignedDistrictIDs;
        }
        return *this;
    }

    std::vector<size_t> &getAssignedDistrictIDs() { return assignedDistrictIDs; }

    friend std::ostream &operator<<(std::ostream &os, const Country &c);
};


#endif //VIRUS_COUNTRY_H
