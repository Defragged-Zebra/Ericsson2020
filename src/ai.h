//
// Created by lukac on 11/6/2020.
//

#include "grid.h"
#include "logic.h"
#include "utils.h"

#ifndef VIRUS_AI_H
#define VIRUS_AI_H


class AI : Logic {
    //this is a copy, NOT a reference/pointer
    static Grid grid;
    static std::priority_queue<Utils::ScoreHolder> districtScores;

    //it is important to COPY the grid
    static void calculateDistrictScoresForNextRound(size_t countryID);

    static const double parameter1;
    static const double parameter2;

    static std::vector<VaccineData>
    chooseDistrictsToHeal(int numberOfVaccinesToDistribute, size_t countryID);

public:
    static void copyGrid(Grid *g) {
        AI::grid = g;
    }

    static std::vector<VaccineData> &
    calculateBackVaccines(std::vector<VaccineData> &back, int &numberOfVaccinesToDistribute, size_t countryID);

    static std::vector<VaccineData> &
    calculatePutVaccines(std::vector<VaccineData> &put, int numberOfVaccinesToDistribute, size_t countryID);

    static void
    calculateChangeByHealingField(const Field *fieldPointer, int &changeInProducedVaccines,
                                  int &changeInDefenseVaccines);
};


#endif //VIRUS_AI_H
