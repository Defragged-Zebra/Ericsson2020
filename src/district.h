//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_DISTRICT_H
#define VIRUS_DISTRICT_H

#include <vector>
#include "field.h"


class District {
    size_t districtID{};
    //size_t assignedCountryID;
    //"cannot store reference in std::vector, it is simply not allowed" - stackoverflow
    //TODO: tárolni a szomszédos kerületeket
    std::vector<Field*> assignedFields;
    bool clear{};
public:
    District() = default;

    District& operator= (const District &d){
        if (this != &d) {
            districtID=d.districtID;
            for (auto assignedField : d.assignedFields) {
                assignedFields.push_back(assignedField);
            }
        }
        return *this;
    }

    District(size_t sectorID, const std::vector<Field*> &assignedFields, bool clear = false) {
        this->districtID = sectorID;
        this->assignedFields = assignedFields;
        this->clear = clear;
    }

    [[nodiscard]] std::vector<Field*> getAssignedFields() const { return assignedFields; }

    [[nodiscard]] size_t getDistrictID() const { return districtID; }

    [[nodiscard]] bool isClear() const { return clear; }

    void setClear(bool value) { this->clear = value; }
    bool operator==(const District&d){
        return this->districtID==d.districtID;
    }
    bool operator!=(const District&d){
        return this->districtID!=d.districtID;
    }

    friend std::ostream &operator<<(std::ostream &os, const District &d);
    void addAssignedField(Field* assignedField){assignedFields.push_back(assignedField);}
    void updateIsClear();
};


#endif //VIRUS_DISTRICT_H
