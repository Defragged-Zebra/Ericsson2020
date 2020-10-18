//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_COUNTRY_H
#define VIRUS_COUNTRY_H


#include <vector>

class Country {
    int id{};
    int totalProductionCapacity{};
    int reserveVaccines{};
    std::vector<int> assignedSectorsID;

    Country(){
        assignedSectorsID=std::vector<int>();
    }
    Country(const Country& c){
        *this = c;
    }
    Country&operator=(const Country& c){
        if(this!=&c){
            id=c.id;
            totalProductionCapacity=c.totalProductionCapacity;
            reserveVaccines = c.reserveVaccines;
            assignedSectorsID=c.assignedSectorsID;
        }
        return *this;
    }

};


#endif //VIRUS_COUNTRY_H
