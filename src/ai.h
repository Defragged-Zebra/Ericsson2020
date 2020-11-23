//
// Created by lukac on 11/6/2020.
//


#ifndef VIRUS_AI_H
#define VIRUS_AI_H

#include "grid.h"
#include "logic.h"
#include "utils.h"
#include "graphalgos.h"
#include "graphalgos.h"
#include "config.h"

class AI {
    //this is a copy
    static Grid *grid2;

    static void calculateDistrictScoresForNextRound(size_t countryID, std::set<ScoreHolder> &districtScores);

    static std::vector<VaccineData> chooseFieldsToVaccinate(int &numberOfVaccinesToDistribute, size_t countryID);

    static void floodDistrict(const Point &p, std::set<Field *> &notVisitedFields, std::vector<Field *> &orderedFields);

    static void calculateScore(std::set<ScoreHolder> &districtScores, const District &district, size_t countryID);

    static Point calculateStartPoint(const std::set<Field *> &fieldsToCalc, size_t countryID);

    static void startFromGridBorder(size_t countryID, std::set<ScoreHolder> &districtScores);

    static void modeB(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
                      std::vector<VaccineData> &fieldsToHealSendBack);

    static void modeA(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
                      std::vector<VaccineData> &fieldsToHealSendBack);

    static void modeC(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
                      std::vector<VaccineData> &fieldsToHealSendBack);

    static void addFieldsToHealWithFlood(int &numberOfVaccinesToDistribute, size_t countryID,
                                         std::vector<VaccineData> &fieldsToHealSendBack,
                                         ScoreHolder maxScoredDistrict);

    static void addFieldsToHealWithDijsktra(int &numberOfVaccinesToDistribute, size_t countryID,
                                            std::vector<VaccineData> &fieldsToHealSendBack,
                                            const ScoreHolder &maxScoredDistrict, std::vector<Point> &startPoints);

    static std::vector<Point> mapAddBorderFields();

    static std::vector<Point> mapAddBorderFieldsForDistrict(size_t districtID);


    static void calculateDistrictScoresWithWannabes(size_t countryID, std::set<ScoreHolder> &districtScores);

    static void calculateWannabeBorder(size_t countryID);

    static Point calculateWannabeStartPoint(const std::set<Field *> &fieldsToCalc, size_t countryID);

    static void addFieldsToWannaHealWithFlood(int &numberOfVaccinesToDistribute, size_t countryID,
                                              std::vector<VaccineData> &fieldsToHealSendBack,
                                              ScoreHolder maxScoredDistrict);

    static void modeWanna(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
                          std::vector<VaccineData> &fieldsToHealSendBack);

    ~AI() { delete grid2; }

public:
    static void copyGrid(Grid *originalGrid) {
        delete AI::grid2;
        AI::grid2 = new Grid(*originalGrid);
    }

    static std::vector<VaccineData> &
    calculateBackVaccines(std::vector<VaccineData> &back, int &numberOfVaccinesToDistribute, size_t countryID);

    static std::vector<VaccineData> &
    calculatePutVaccines(std::vector<VaccineData> &put, int &numberOfVaccinesToDistribute, size_t countryID);
};


#endif //VIRUS_AI_H
