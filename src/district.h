//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_DISTRICT_H
#define VIRUS_DISTRICT_H

#include <vector>
#include "field.h"

class District {
private:
    int sectorID;
    std::vector<Field *> fields;
    bool clear;
public:
    District(int sectorID, std::vector<Field *> fields, bool clear);

    std::vector<Field *> getFields() const;

    int getID() const;

    bool isClear();
};


#endif //VIRUS_DISTRICT_H
