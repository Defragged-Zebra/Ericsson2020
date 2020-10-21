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
    size_t assignedCountryID;
    std::vector<size_t> assignedFieldIDs;
    bool clear;
public:
    District(size_t sectorID, std::vector<size_t> assignedFieldIDs, bool clear);

    std::vector<size_t> &getAssignedFields() { return assignedFieldIDs; }

    size_t getDistrictID() const { return districtID; }

    bool isClear() const { return clear; }


    friend std::ostream &operator<<(std::ostream &os, const District &d);
};


#endif //VIRUS_DISTRICT_H