//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_COUNTRY_H
#define VIRUS_COUNTRY_H


#include <vector>
#include <iostream>
#include <set>
#include "utils.h"
#include "district.h"

//TODO refactor
class Country {
    size_t countryID{};
    int totalProductionCapacity{};
    int reservedVaccines{};
    //similar to field
    std::set<District*> assignedDistrictPointers;
    std::set<Point> vaccinatedFields;
public:
    Country() =default;
    Country(size_t ID, size_t TPC, size_t RV){
        this->countryID = ID;
        this->totalProductionCapacity = TPC;
        this->reservedVaccines =RV;
        this->assignedDistrictPointers = std::set<District*>();
    }

    Country(size_t ID, const std::set<District*> &districts) {
        countryID = ID;
        assignedDistrictPointers = districts;
    }

    Country(const Country &c) {
        *this = c;
    }

    Country &operator=(const Country &c) {
        if (this != &c) {
            this->countryID = c.countryID;
            this->totalProductionCapacity = c.totalProductionCapacity;
            this->reservedVaccines = c.reservedVaccines;
            this->assignedDistrictPointers = c.assignedDistrictPointers;
            this->vaccinatedFields = c.vaccinatedFields;
        }
        return *this;
    }

    [[nodiscard]] std::set<District*> getAssignedDistrictIDs() const { return assignedDistrictPointers; }
    void addAssignedDistrictID(District* districtPointer){assignedDistrictPointers.insert(districtPointer);}

    friend std::ostream &operator<<(std::ostream &os, const Country &c);

    [[nodiscard]] int getTotalProductionCapacity() const { return totalProductionCapacity; }

    void setTotalProductionCapacity(int tpc) { totalProductionCapacity = tpc; }

    [[nodiscard]] int getReserveVaccines() const { return reservedVaccines; }
    [[nodiscard]] size_t getCountryID() const {return countryID;}
    void setReserveVaccines(int rv) { reservedVaccines = rv; }
    bool isFieldInCountry(size_t ID);
    void addToVaccinatedFields(const Point& p){
        vaccinatedFields.insert(p);
    }
    [[nodiscard]] bool isNeighbourVaccinatedFields(const Point& p)const{
        if(vaccinatedFields.empty()){
            return p.getY()==0 or p.getX()==0 or p.getY()==Point::getGridHeight()-1 or p.getX()==
                                                                                               Point::getGridWidth() - 1;
        }else{
            size_t centerY = p.getY();
            size_t centerX = p.getX();
            Point coordinates[4] = {{centerY,     centerX - 1},
                                    {centerY - 1, centerX},
                                    {centerY + 1, centerX},
                                    {centerY,     centerX + 1}};

            for (const auto &selected : coordinates) {
                if(!selected.withinBounds())continue;
                 auto it = vaccinatedFields.find(selected);
                 if(it!=vaccinatedFields.end()) return true;

            }

        }
        return false;
    }
};


#endif //VIRUS_COUNTRY_H
