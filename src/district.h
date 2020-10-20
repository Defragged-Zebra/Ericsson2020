//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_DISTRICT_H
#define VIRUS_DISTRICT_H

#include <vector>
#include "field.h"

class District {
private:
    int districtID;
    int assignedCountryID;
    std::vector<int> assignedFieldIDs;
    bool clear;
public:
    District(int sectorID, std::vector<int> fields, bool clear);

    std::vector<int> getAssignedFields() const;

    int getDistrictID() const;

    bool isClear () const;
};


#endif //VIRUS_DISTRICT_H
