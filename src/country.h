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
    std::set<size_t> assignedDistricts{};
    std::set<size_t> wannabeDistricts{};
    std::map<size_t, std::set<Point>> vaccinatedFields{};
    std::map<size_t, std::set<Point>> wannabeVaccinatedFields{};
    std::set<Point> border{};
    std::set<Point> wannabeBorder{};
public:
    Country() = default;

    Country(size_t ID, size_t TPC, size_t RV) {
        this->countryID = ID;
        this->totalProductionCapacity = TPC;
        this->reservedVaccines = RV;
        this->assignedDistricts = std::set<size_t>();
    }

    Country(size_t ID, const std::set<size_t> &districts) {
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
            this->wannabeDistricts = c.wannabeDistricts;
            this->wannabeBorder = c.wannabeBorder;
            this->wannabeVaccinatedFields = c.wannabeVaccinatedFields;
        }
        return *this;
    }

    [[nodiscard]] std::set<size_t> getAssignedDistricts() const { return assignedDistricts; }

    void addAssignedDistrict(size_t district) {
        assignedDistricts.insert(district);
        vaccinatedFields[district].clear();
    }

    friend std::ostream &operator<<(std::ostream &os, const Country &c);

    [[nodiscard]] int getTotalProductionCapacity() const { return totalProductionCapacity; }

    void setTotalProductionCapacity(int tpc) { totalProductionCapacity = tpc; }

    [[nodiscard]] int getReserveVaccines() const { return reservedVaccines; }

    [[nodiscard]] size_t getCountryID() const { return countryID; }

    void setReserveVaccines(int rv) { reservedVaccines = rv; }

    //bool isFieldInCountry(size_t ID);

    void addToVaccinatedFields(const Point &p, size_t districtID) {
        vaccinatedFields[districtID].insert(p);
    }
    void addToWannabeVaccinatedFields(const Point &p, size_t districtID) {
        wannabeVaccinatedFields[districtID].insert(p);
    }

    [[nodiscard]] bool isNeighbourToVaccinatedField(const Point &p) const;

    void setBorder(std::set<Point> &bord) {
        border.clear();
        border = bord;
    }

    void setWannabeBorder(std::set<Point> &bord) {
        wannabeBorder.clear();
        wannabeBorder = bord;
    }

    std::set<Point> getBorder() { return border; }

    std::set<Point> getWannabeBorder() { return wannabeBorder; }

    void thereAreNoVaccinatedFieldsHere(std::map<size_t, std::set<Point>> &notVaccinatedFields) {
        for (const auto &nvd:notVaccinatedFields) {
            auto &vd = vaccinatedFields[nvd.first];
            for (const auto &nvf:nvd.second) {
                auto it = std::find(vd.begin(), vd.end(), nvf);
                if (it != vd.end()) {
                    vd.erase(it);
                }
            }
        }
    }
    void thereAreNoWannabeVaccinatedFieldsHere(std::map<size_t, std::set<Point>> &notWannabeVaccinatedFields) {
        for (const auto &nvd:notWannabeVaccinatedFields) {
            auto &vd = wannabeVaccinatedFields[nvd.first];
            for (const auto &nvf:nvd.second) {
                auto it = std::find(vd.begin(), vd.end(), nvf);
                if (it != vd.end()) {
                    vd.erase(it);
                }
            }
        }
    }

    bool hasDistrict() { return !assignedDistricts.empty(); }

    [[nodiscard]] std::set<size_t> getWannabeDistricts() const { return wannabeDistricts; }

    void addWannabeDistrict(size_t district) {
        wannabeDistricts.insert(district);
        wannabeVaccinatedFields[district].clear();
    }

    [[nodiscard]] bool isNeighbourToWannabeVaccinatedField(const Point &p) const;
};


#endif //VIRUS_COUNTRY_H
