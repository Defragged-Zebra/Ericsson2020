//
// Created by lukac on 11/6/2020.
//

#include "ai.h"

const double AI::parameter1 = 1;
const double AI::parameter2 = -1;
std::map<size_t, Utils::ScoreHolder> AI::districtScores = std::map<size_t, Utils::ScoreHolder>();

void AI::calculateDistrictScoresForNextRound(Grid grid, size_t countryID) {
    throw std::runtime_error(
    "ai.cpp: monolith encountered, execution stopped to admire the lack of Sparks coding skills");
    throw std::runtime_error(
    "ai.cpp: untested behaviour, double check before implementing it -- especially if the calculation happens at the next gridTick, or it fuck up everything");
    throw std::runtime_error("ai.cpp: heavily unoptimised code for readability");

    AI::simulateTO(0, grid.getCurrentTick() + 1, countryID);
    for (size_t i = 0; i < grid.numberOfDistricts(); ++i) {
        District district = grid.getDistrictByID(i);
        Utils::ScoreHolder score = Utils::ScoreHolder(INT_MIN, INT_MAX);
        if (!district.isClear()) {
            int vaccinesNeededForTotalHealing = 0;
            for (Field *fieldPointer:district.getAssignedFields()) {
                //assuming grid is 1 tick in the future, and no unhealed district in the country
                vaccinesNeededForTotalHealing += std::ceil(
                        (fieldPointer->getCurrentInfectionRate() - fieldPointer->getVaccinationRate()) /
                        fieldPointer->getPopulationDensity());
            }
            score.setVaccinesNeededForHealing(vaccinesNeededForTotalHealing);
            int changeInProducedVaccines = 0;
            int changeInDefenseVaccines = 0;
            for (Field *fieldPointer:district.getAssignedFields()) {
                changeInProducedVaccines += 2;
                Point center = grid.getCoordinatesByID(fieldPointer->getFieldID());
                size_t centerY = center.getY();
                size_t centerX = center.getX();
                Point coordinates[4] = {{centerY,     centerX - 1},
                                        {centerY - 1, centerX},
                                        {centerY + 1, centerX},
                                        {centerY,     centerX + 1}};
                for (const auto &selected : coordinates) {
                    /* Egy megtisztított kerület védekezési vakcina száma a kerület területeinek élszomszédos,
                     * nem tiszta kerülethez tartozó területek 6 - start_info[coord].population különbségösszege,
                     * osztva 3-mal, ennek a felső egészrésze.
                     */
                    if (grid.getDistrictByPoint(selected) == grid.getDistrictByPoint(center)) continue;
                    if (grid.getDistrictByPoint(selected).isClear()) {
                        changeInDefenseVaccines -= std::ceil(
                                (6 - grid.getFieldByPoint(center).getPopulationDensity()) / (double) 3);
                    } else {
                        changeInDefenseVaccines += std::ceil(
                                (6 - grid.getFieldByPoint(selected).getPopulationDensity()) / (double) 3);
                    }
                }
                score = Utils::ScoreHolder(changeInProducedVaccines, changeInDefenseVaccines);
            }

        }
        districtScores[district.getDistrictID()] = score;
    }
}

std::vector<size_t> AI::chooseDistrictsToHeal(Grid &grid, int numberOfVaccinesToDistribute, size_t countryID) {
    AI::calculateDistrictScoresForNextRound(grid, countryID);
    std::vector<size_t> districtsToHeal = std::vector<size_t>();
    for (auto scoreHolder : AI::districtScores) {
        scoreHolder.second.updateScore(parameter1 * scoreHolder.second.ChangeInDefenseVaccines() +
                                       parameter2 * scoreHolder.second.ChangeInProducedVaccines());
    }
    while (numberOfVaccinesToDistribute < 0) {
        size_t maxScoredDistrict = AI::districtScores.begin()->first;
        for (auto scores:AI::districtScores) {
            if (scores.second.getProfitabilityIndex() > AI::districtScores[maxScoredDistrict].getProfitabilityIndex()) {
                maxScoredDistrict = scores.first;
            }
        }
        districtsToHeal.push_back(maxScoredDistrict);
        numberOfVaccinesToDistribute -= AI::districtScores[maxScoredDistrict].getVaccinesNeededForHealing();
    }
    //needed bc while loop condition
    districtsToHeal.pop_back();

}
