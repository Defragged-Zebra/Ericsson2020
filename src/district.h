//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_DISTRICT_H
#define VIRUS_DISTRICT_H

#include <vector>
#include "field.h"
#include <algorithm>


class District {
    size_t districtID ;
    //TODO: ennel valami jobbat (am meg atnezni hogy tuti kell-e ez ide)
    size_t assignedCountryID= -1;
    //"cannot store reference in std::vector, it is simply not allowed" - stackoverflow
    std::vector<Field *> assignedFields;
    std::vector<size_t> neighbourDistricts;
    bool clear{};
public:
    District() = default;

    District &operator=(const District &d) {
        if (this != &d) {
            this->districtID = d.districtID;
            this->assignedFields = d.assignedFields;
            this->neighbourDistricts = d.neighbourDistricts;
            this->assignedCountryID = d.assignedCountryID;
        }
        return *this;
    }

    District(size_t sectorID, const std::vector<Field *> &assignedFields, const std::vector<size_t> &neighbourDistricts,
             bool clear = false) {
        this->districtID = sectorID;
        this->assignedFields = assignedFields;
        this->clear = clear;
        this->neighbourDistricts = neighbourDistricts;
    }

    [[nodiscard]] std::vector<Field *> getAssignedFields() const { return assignedFields; }

    [[nodiscard]] size_t getDistrictID() const { return districtID; }

    [[nodiscard]] bool isClear() const { return clear; }

    void setClear(bool value) { this->clear = value; }

    bool operator==(const District &d) const {
        return this->districtID == d.districtID;
    }

    bool operator!=(const District &d) const {
        return this->districtID != d.districtID;
    }

    friend std::ostream &operator<<(std::ostream &os, const District &d);

    void addAssignedField(Field *assignedField) { assignedFields.push_back(assignedField); }

    bool updateIsClear();

    [[nodiscard]] const std::vector<size_t> &getNeighbourDistricts() const { return neighbourDistricts; }

    void addNeighbourDistrict(size_t neighbour) { neighbourDistricts.push_back(neighbour); }

    void simplifyNeighbourDistrictVector();

    void setAssignedCountryID(size_t countryID) { assignedCountryID = countryID; }
    bool isFieldInDistrict(size_t fieldID);
};


#endif //VIRUS_DISTRICT_H
