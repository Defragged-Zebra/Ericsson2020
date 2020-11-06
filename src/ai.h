//
// Created by lukac on 11/6/2020.
//

#include "grid.h"
#include "logic.h"
#include <climits>
#include "utils.h"

#ifndef VIRUS_AI_H
#define VIRUS_AI_H


class AI:public Logic {
    static std::map<size_t, Utils::ScoreHolder> districtScores;
    //it is important to COPY the grid
    void calculateDistrictScoresForNextRound(Grid grid, size_t countryID);

    static const double parameter1;
    static const double parameter2;
public:
    std::vector<size_t> chooseDistrictsToHeal(Grid &grid, int numberOfVaccinesToDistribute, size_t countryID);
};


#endif //VIRUS_AI_H
