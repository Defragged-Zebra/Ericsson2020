//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_COUNTRY_H
#define VIRUS_COUNTRY_H


#include <vector>
#include <iostream>
#include <set>
#include <functional>
#include "utils.h"
#include "district.h"

//TODO refactor
class Country {
    size_t countryID{};
    int totalProductionCapacity{};
    int reservedVaccines{};
    //similar to field
    std::set<District*> assignedDistricts;
    std::set<Point> vaccinatedFields;
    std::set<Point> border;
public:
    Country() =default;
    Country(size_t ID, size_t TPC, size_t RV){
        this->countryID = ID;
        this->totalProductionCapacity = TPC;
        this->reservedVaccines =RV;
        this->assignedDistricts = std::set<District*>();
    }

    Country(size_t ID, const std::set<District*> &districts) {
        countryID = ID;
        assignedDistricts = districts;
    }

    Country(const Country &c) {
        *this = c;
    }

    Country &operator=(const Country &c) {
        if (this != &c) {
            this->countryID = c.countryID;
            this->totalProductionCapacity = c.totalProductionCapacity;
            this->reservedVaccines = c.reservedVaccines;
            this->assignedDistricts = c.assignedDistricts;
            this->vaccinatedFields = c.vaccinatedFields;
            this->border = c.border;
        }
        return *this;
    }

    [[nodiscard]] std::set<District*> getAssignedDistricts() const { return assignedDistricts; }
    void addAssignedDistrict(District* district){assignedDistricts.insert(district);}

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
    [[nodiscard]] bool isNeighbourToVaccinatedField(const Point& p)const;
    void setBorder(std::set<Point>& bord){
        border.clear();
        border=bord;
    }
    std::set<Point> getBorder(){
        return border;
    }
};


#endif //VIRUS_COUNTRY_H
