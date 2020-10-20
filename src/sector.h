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
    std::vector<Field*> fields;
public:
    Sector(std::vector<Field*> fields);
    std::vector<Field*> getFields () const;
};


#endif //VIRUS_SECTOR_H
