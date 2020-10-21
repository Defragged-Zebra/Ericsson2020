//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_COUNTRY_H
#define VIRUS_COUNTRY_H


#include <vector>

class Country {
    size_t ID{};
    int totalProductionCapacity{};
    int reserveVaccines{};
    std::vector<size_t> assignedDistrictIDs;
public:
    Country(){
        assignedDistrictIDs=std::vector<size_t>();
    }
    Country(const Country& c){
        *this = c;
    }
    Country&operator=(const Country& c){
        if(this!=&c){
            ID=c.ID;
            totalProductionCapacity=c.totalProductionCapacity;
            reserveVaccines = c.reserveVaccines;
            assignedDistrictIDs=c.assignedDistrictIDs;
        }
        return *this;
    }

};


#endif //VIRUS_COUNTRY_H
