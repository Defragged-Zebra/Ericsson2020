//
// Created by lukac on 11/6/2020.
//

#include "grid.h"
#include "logic.h"
#include "utils.h"

#ifndef VIRUS_AI_H
#define VIRUS_AI_H


class AI : Logic {
    static std::map<size_t, Utils::ScoreHolder> districtScores;

    //it is important to COPY the grid
    static void calculateDistrictScoresForNextRound(Grid grid, size_t countryID);

    static void reset();

    static const double parameter1;
    static const double parameter2;

    static std::vector<VaccineData> chooseDistrictsToHeal(Grid &grid, int numberOfVaccinesToDistribute, size_t countryID);

public:

    static std::vector<VaccineData> &
    calculateBackVaccines(std::vector<VaccineData> &back, size_t tickID, Grid &grid, int &numberOfVaccinesToDistribute,
                          size_t countryID);

    static std::vector<VaccineData> &calculatePutVaccines(std::vector<VaccineData> &put, size_t tickID, Grid &grid, int numberOfVaccinesToDistribute,
                                                          size_t countryID);
};


#endif //VIRUS_AI_H
