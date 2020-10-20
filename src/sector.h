//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_SECTOR_H
#define VIRUS_SECTOR_H

#include <vector>
#include "field.h"

class Sector {
private:
    int sectorID;
    std::vector<Field *> fields;
    bool clear;
public:
    Sector(int sectorID, std::vector<Field *> fields, bool clear);

    std::vector<Field *> getFields() const;

    int getID() const;

    bool isClear();
};


#endif //VIRUS_SECTOR_H
