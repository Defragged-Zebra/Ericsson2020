//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_DISTRICT_H
#define VIRUS_DISTRICT_H

#include <vector>
#include "field.h"

class District {
private:
    size_t districtID;
    //size_t assignedCountryID;
    std::vector<size_t> assignedFieldIDs;
    bool clear;
public:
    District() = default;

    District(size_t sectorID, const std::vector<size_t> &assignedFieldIDs, bool clear = false) {
        this->districtID = sectorID;
        this->assignedFieldIDs = assignedFieldIDs;
        this->clear = clear;
    }

    std::vector<size_t> getAssignedFields() const { return assignedFieldIDs; }

    size_t getDistrictID() const { return districtID; }

    bool isClear() const { return clear; }

    void setClear(bool clear) { this->clear = clear; }
    bool operator==(const District&d){
        return this->districtID==d.districtID;
    }
    bool operator!=(const District&d){
        return this->districtID!=d.districtID;
    }

    friend std::ostream &operator<<(std::ostream &os, const District &d);
};


#endif //VIRUS_DISTRICT_H
