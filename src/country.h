//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_COUNTRY_H
#define VIRUS_COUNTRY_H


#include <vector>
#include <iostream>

//TODO refactor
class Country {
    size_t countryID{};
    int totalProductionCapacity{};
    int reservedVaccines{};
    std::vector<size_t> assignedDistrictIDs;
public:
    Country() {
        assignedDistrictIDs = std::vector<size_t>();
    }

    Country(size_t ID, const std::vector<size_t> &districts) {
        countryID = ID;
        assignedDistrictIDs = districts;
    }

    Country(const Country &c) {
        *this = c;
    }

    Country &operator=(const Country &c) {
        if (this != &c) {
            countryID = c.countryID;
            totalProductionCapacity = c.totalProductionCapacity;
            reservedVaccines = c.reservedVaccines;
            assignedDistrictIDs = c.assignedDistrictIDs;
        }
        return *this;
    }

    [[nodiscard]] std::vector<size_t> getAssignedDistrictIDs() const { return assignedDistrictIDs; }

    friend std::ostream &operator<<(std::ostream &os, const Country &c);

    [[nodiscard]] int getTotalProductionCapacity() const { return totalProductionCapacity; }

    void setTotalProductionCapacity(int tpc) { totalProductionCapacity = tpc; }

    [[nodiscard]] int getReserveVaccines() const { return reservedVaccines; }

    void setReserveVaccines(int rv) { reservedVaccines = rv; }
};


#endif //VIRUS_COUNTRY_H
