//
// Created by lukac on 11/6/2020.
//

#include "grid.h"
#include "logic.h"
#include "utils.h"

#ifndef VIRUS_AI_H
#define VIRUS_AI_H


class AI {
    //this is a copy, NOT a reference/pointer
    static Grid grid2;

    //it is important to COPY the grid
    static void calculateDistrictScoresForNextRound(size_t countryID, std::vector<ScoreHolder> &districtScores);

    static std::vector<VaccineData>
    chooseFieldsToVaccinate(int numberOfVaccinesToDistribute, size_t countryID);
    static void floodDistrict(const Point& p, std::vector<VaccineData> &notVisitedFields, std::vector<VaccineData>& orderedFields);

public:
    static void copyGrid(Grid *originalGrid) {
        AI::grid2 = originalGrid;
    }

    static std::vector<VaccineData> &
    calculateBackVaccines(std::vector<VaccineData> &back, int &numberOfVaccinesToDistribute, size_t countryID);

    static std::vector<VaccineData> &
    calculatePutVaccines(std::vector<VaccineData> &put, int numberOfVaccinesToDistribute, size_t countryID);

    static void
    calculateChangeByHealingField(const Field *fieldPointer, int &changeInProducedVaccines,
                                  int &changeInDefenseVaccines);

    static void CalculateScore(std::vector<ScoreHolder> &districtScores, const District &district);
};


#endif //VIRUS_AI_H
