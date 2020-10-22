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
    int currentInfectionValue;
    int vaccinationRate;
    int populationDensity;
    std::map<int, int> storedVaccines;
    std::deque<int> lastInfectionValues;
    size_t assignedDistrictID;
    size_t numberOfStoredPastValues;
public:
    Field() = default;

    Field(const int fieldID, const int assignedDistrictID, const int currentInfectionValue, const int vaccinationRate,
          const int populationDensity, size_t numberOfStoredPastValues) {
        this->fieldID = fieldID;
        this->assignedDistrictID = assignedDistrictID;
        this->currentInfectionValue = currentInfectionValue;
        this->vaccinationRate = vaccinationRate;
        this->populationDensity = populationDensity;
        this->storedVaccines = std::map<int, int>();
        this->lastInfectionValues = std::deque<int>();
        this->numberOfStoredPastValues = numberOfStoredPastValues;
    }

    Field(const Field &f) {
        *this = f;
    }

    Field &operator=(const Field &f) {
        if (this != &f) {
            this->fieldID = f.fieldID;
            this->lastInfectionValues = f.lastInfectionValues;
            this->currentInfectionValue = f.currentInfectionValue;
            this->storedVaccines = f.storedVaccines;
            this->populationDensity = f.populationDensity;
            this->vaccinationRate = f.vaccinationRate;
        }
        return *this;
    }

    size_t getFieldID() const { return this->fieldID; }

    friend std::ostream &operator<<(std::ostream &os, const Field &f);

    std::deque<int> &getLastInfectionValues() { return lastInfectionValues; }

    int getPopulationDensity() const { return populationDensity; }

    size_t getAssignedDistrictID() const { return assignedDistrictID; }

    void updateVaccination(int value);

    void updateInfection(int value);

    int getCurrentInfectionValue() { return currentInfectionValue; }

    int getVaccinationRate() { return vaccinationRate; }

    void setNumberOfPastValues(size_t values) { numberOfStoredPastValues = values; }
};


#endif //VIRUS_FIELD_H
