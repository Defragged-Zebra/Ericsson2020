//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_FIELD_H
#define VIRUS_FIELD_H


#include <map>
#include <queue>
#include <sstream>
#include <cmath>

class Field {
    size_t fieldID{};
    int infectionRate{};
    int vaccinationRate{};
    int populationDensity{};
    size_t assignedDistrictID{};
    std::map<size_t, int> storedVaccines{}; //countryID, stored amount
    //this stores the history of the infectionRate-s
    std::deque<int> lastInfectionRates{};
    //this stores the history of the infection values returned by the Logic::calculateInfectionValue()
    std::deque<int> lastInfectionValues{};
    size_t lastInfRateLen{};
public:
    Field() = default;
    friend std::ostream &operator<<(std::ostream &os, const Field &f){
        os<<"FieldID: "<<f.getFieldID()<<std::endl;
        os<<"InfRate: "<<f.getCurrentInfectionRate()<<std::endl;
        os<<"VaccRate: "<<f.getVaccinationRate()<<std::endl;
        os<<"PopulDensity: "<<f.getPopulationDensity()<<std::endl;
        os<<"AssigDistrID: "<<f.getAssignedDistrictID()<<std::endl;
        return os;
    }

    Field(const int fieldID, const int assignedDistrictID, const int currentInfectionValue, const int vaccinationRate,
          const int populationDensity, size_t numberOfStoredPastValues) {
        this->fieldID = fieldID;
        this->assignedDistrictID = assignedDistrictID;
        this->infectionRate = currentInfectionValue;
        this->vaccinationRate = vaccinationRate;
        this->populationDensity = populationDensity;
        this->storedVaccines = std::map<size_t, int>();
        this->lastInfectionRates = std::deque<int>();
        this->lastInfectionValues = std::deque<int>();
        this->lastInfectionRates.push_back(currentInfectionValue);
        this->lastInfectionValues.push_back(currentInfectionValue > 0 ? 1 : 0);
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

    [[nodiscard]] size_t getFieldID() const { return this->fieldID; }

    friend std::ostream &operator<<(std::ostream &os, const Field &f);

    std::deque<int> &getLastInfectionRates() { return lastInfectionRates; }

    std::deque<int> &getLastInfectionValues() { return lastInfectionValues; }

    [[nodiscard]] int getPopulationDensity() const { return populationDensity; }

    [[nodiscard]] size_t getAssignedDistrictID() const { return assignedDistrictID; }

    void updateVaccination(int value);

    void updateInfection(int value);

    [[nodiscard]] int getCurrentInfectionRate() const { return infectionRate; }

    [[nodiscard]] int getVaccinationRate() const { return vaccinationRate; }

    [[nodiscard]] bool isClear() const { return infectionRate == 0; }

    [[nodiscard]] std::map<size_t, int> getStoredVaccines() const { return storedVaccines; }

    void updateRemainingVaccines(int vaccinated);

    void callBackVaccines(int vaccines, size_t countryID);

    void pushVaccines(int vaccines, size_t countryID);

    friend bool operator<(const Field &f1, const Field &f2) {
        return f1.getFieldID() < f2.getFieldID();
    }

    [[nodiscard]] int vaccinesToPutMinimal(size_t countryID) {
        //don't check for clear, it'll break other stuff
        if (storedVaccines[countryID] > 0) return 0;
        else return 6 - populationDensity;
    }

    [[nodiscard]] int vaccinesToPutForTotalHealing(size_t countryID) {
        return std::max((int) std::ceil((infectionRate - vaccinationRate) / populationDensity),
                        vaccinesToPutMinimal(countryID));
    }
    ~Field()= default;
};


#endif //VIRUS_FIELD_H
