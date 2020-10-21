//
// Created by woranhun on 2020. 10. 17..
//

#include "district.h"

District::District(size_t sectorID, std::vector<size_t> assignedFieldIDs, bool clear = false) {
    this->districtID = sectorID;
    this->assignedFieldIDs = assignedFieldIDs;
    this->clear = clear;
}
