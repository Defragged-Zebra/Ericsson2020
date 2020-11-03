//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_DISTRICT_H
#define VIRUS_DISTRICT_H

#include <vector>
#include "field.h"

#include <vector>
#include <stdexcept>
#include "field.h"
#include "utils.h"
#include "country.h"
#include "district.h"
#include <algorithm>

class District {
    size_t districtID;
    //size_t assignedCountryID;
    std::vector<Field> fields;
    std::vector<Field&> assignedFields;
    bool clear;
public:
    District() = default;

    District(size_t sectorID, const std::vector<Field&> &assignedFields, bool clear = false) {
        this->districtID = sectorID;
        this->assignedFields = assignedFields;
        this->clear = clear;
    }

    std::vector<size_t> getAssignedFields() const { return assignedFields; }

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

    void updateIsClear();
};


#endif //VIRUS_DISTRICT_H
