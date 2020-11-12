//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_COUNTRY_H
#define VIRUS_COUNTRY_H


#include <vector>
#include <iostream>
#include "utils.h"
#include "district.h"

//TODO refactor
class Country {
    size_t countryID{};
    int totalProductionCapacity{};
    int reservedVaccines{};
    //similar to field
    std::vector<District*> assignedDistrictPointers;
public:
    Country() {
        assignedDistrictPointers = std::vector<District*>();
    }
    Country(size_t ID, size_t TPC, size_t RV){
        this->countryID = ID;
        this->totalProductionCapacity = TPC;
        this->reservedVaccines =RV;
        this->assignedDistrictPointers = std::vector<District*>();
    }

    Country(size_t ID, const std::vector<District*> &districts) {
        countryID = ID;
        assignedDistrictPointers = districts;
    }

    Country(const Country &c) {
        *this = c;
    }

    Country &operator=(const Country &c) {
        if (this != &c) {
            this->countryID = c.countryID;
            this->totalProductionCapacity = c.totalProductionCapacity;
            this->reservedVaccines = c.reservedVaccines;
            this->assignedDistrictPointers = c.assignedDistrictPointers;
        }
        return *this;
    }

    [[nodiscard]] std::vector<District*> getAssignedDistrictIDs() const { return assignedDistrictPointers; }
    void addAssignedDistrictID(District* districtPointer){assignedDistrictPointers.push_back(districtPointer);}

    friend std::ostream &operator<<(std::ostream &os, const Country &c);

    [[nodiscard]] int getTotalProductionCapacity() const { return totalProductionCapacity; }

    void setTotalProductionCapacity(int tpc) { totalProductionCapacity = tpc; }

    [[nodiscard]] int getReserveVaccines() const { return reservedVaccines; }
    [[nodiscard]] size_t getCountryID() const {return countryID;}
    void setReserveVaccines(int rv) { reservedVaccines = rv; }
    bool isFieldInCountry(size_t ID);
};


#endif //VIRUS_COUNTRY_H
