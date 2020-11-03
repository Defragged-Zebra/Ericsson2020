//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_FIELD_H
#define VIRUS_FIELD_H


#include <map>
#include <queue>
#include <sstream>
#include "district.h"

class Field {
    size_t fieldID;
    int infectionRate;
    int vaccinationRate;
    int populationDensity;
    size_t assignedDistrictID;
    std::map<int, int> storedVaccines;
    //this stores the history of the infectionRate-s
    std::deque<int> lastInfectionRates;
    //this stores the history of the infection values returned by the Logic::calculateInfectionValue()
    std::deque<int> lastInfectionValues;
    size_t lastInfRateLen;
public:
    Field() = default;

    Field(const int fieldID, const int assignedDistrictID, const int currentInfectionValue, const int vaccinationRate,
          const int populationDensity, size_t numberOfStoredPastValues) {
        this->fieldID = fieldID;
        this->assignedDistrictID = assignedDistrictID;
        this->infectionRate = currentInfectionValue;
        this->vaccinationRate = vaccinationRate;
        this->populationDensity = populationDensity;
        this->storedVaccines = std::map<int, int>();
        this->lastInfectionRates = std::deque<int>();
        this->lastInfectionValues = std::deque<int>();
        lastInfectionRates.push_back(currentInfectionValue);
        lastInfectionValues.push_back(currentInfectionValue > 0 ? 1 : 0);
        this->lastInfRateLen = numberOfStoredPastValues;
    }

    Field(const Field &f) {
        *this = f;
    }

    Field &operator=(const Field &f) {
        if (this != &f) {
            this->fieldID = f.fieldID;
            this->lastInfectionRates = f.lastInfectionRates;
            this->infectionRate = f.infectionRate;
            this->storedVaccines = f.storedVaccines;
            this->populationDensity = f.populationDensity;
            this->vaccinationRate = f.vaccinationRate;
            this->assignedDistrictID = f.assignedDistrictID;
            this->lastInfRateLen = f.lastInfRateLen;
            this->lastInfectionValues = f.lastInfectionValues;
        }
        return *this;
    }

    size_t getFieldID() const { return this->fieldID; }

    friend std::ostream &operator<<(std::ostream &os, const Field &f);

    std::deque<int> &getLastInfectionRates() { return lastInfectionRates; }

    std::deque<int> &getLastInfectionValues() { return lastInfectionValues; }

    int getPopulationDensity() const { return populationDensity; }

    size_t getAssignedDistrictID() const { return assignedDistrictID; }

    void updateVaccination(int value);

    void updateInfection(int value);

    int getCurrentInfectionRate() const { return infectionRate; }

    int getVaccinationRate() const { return vaccinationRate; }

    bool isClear() { return infectionRate == 0 ? true : false; }
};


#endif //VIRUS_FIELD_H
