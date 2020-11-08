//
// Created by lukac on 11/6/2020.
//

#include "ai.h"

const double AI::parameter1 = 1;
const double AI::parameter2 = -1;
std::map<size_t, Utils::ScoreHolder> AI::districtScores = std::map<size_t, Utils::ScoreHolder>();
uint64_t fuckCpp[4] = {0};
Grid AI::grid = Grid(0, 0, fuckCpp);

void AI::calculateDistrictScoresForNextRound(size_t countryID) {
//    throw std::runtime_error(
//    "ai.cpp: monolith encountered, execution stopped to admire the lack of Sparks coding skills");
//    throw std::runtime_error(
//    "ai.cpp: untested behaviour, double check before implementing it -- especially if the calculation happens at the next gridTick, or it fuck up everything");
//    throw std::runtime_error("ai.cpp: heavily unoptimised code for readability");
    AI::simulateTO(0, grid.getCurrentTick() + 1, countryID);
    for (size_t i = 0; i < grid.numberOfDistricts(); ++i) {
        District district = grid.getDistrictByID(i);
        Utils::ScoreHolder score = Utils::ScoreHolder();
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

std::vector<VaccineData> AI::chooseDistrictsToHeal(Grid &grid, int numberOfVaccinesToDistribute, size_t countryID) {
    reset();
    AI::calculateDistrictScoresForNextRound(countryID);
    std::vector<VaccineData> districtsToHeal = std::vector<VaccineData>();
    for (auto scoreHolder : AI::districtScores) {
        scoreHolder.second.updateScore(parameter1 * scoreHolder.second.ChangeInDefenseVaccines() +
                                       parameter2 * scoreHolder.second.ChangeInProducedVaccines());
    }
    while (numberOfVaccinesToDistribute < 0) {
        size_t maxScoredDistrict = AI::districtScores.begin()->first;
        //how many vaccines needed for healing
        int amount = 0;
        for (auto scores:AI::districtScores) {
            if (scores.second.getProfitabilityIndex() > AI::districtScores[maxScoredDistrict].getProfitabilityIndex()) {
                maxScoredDistrict = scores.first;
                amount = scores.second.getVaccinesNeededForHealing();
            }
        }
        VaccineData vc = VaccineData(grid.getCoordinatesByID(maxScoredDistrict), amount);
        districtsToHeal.push_back(vc);
        numberOfVaccinesToDistribute -= AI::districtScores[maxScoredDistrict].getVaccinesNeededForHealing();
    }
    //needed bc while loop condition.pop_back();
    //TODO: look after I have internet access, if it's passed on reference will it be fucked-up?
    return districtsToHeal;
}

void AI::reset() {
    AI::districtScores.clear();
}

std::vector<VaccineData> &
AI::calculateBackVaccines(std::vector<VaccineData> &back, size_t tickID, int &numberOfVaccinesToDistribute,
                          size_t countryID) {
    //TODO: a healthy integer underflow happens here if TickID is 0, don't mind it

    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            std::map<size_t, int> &allStoredVaccines = grid.getFieldByPoint(Point(y, x)).getStoredVaccines();
            int countryStoredVaccines;
            try { countryStoredVaccines = allStoredVaccines.at(countryID); }
            catch (std::out_of_range &exc) { countryStoredVaccines = 0; }
            //Egy területről az összes tartalék vakcinát nem lehet visszavenni, legalább 1 egységnyit ott kell hagyni.
            if (countryStoredVaccines > 1) {
                back.emplace_back(Point(y, x), countryStoredVaccines - 1);
                numberOfVaccinesToDistribute += countryStoredVaccines - 1;
            }
        }

    }
    //return only the last
    return back;
}

std::vector<VaccineData> &
AI::calculatePutVaccines(std::vector<VaccineData> &put, size_t tickID, int numberOfVaccinesToDistribute,
                         size_t countryID) {

    //overwrite, bc you need to return the last values (thx Ericsson )
    put = chooseDistrictsToHeal(grid, numberOfVaccinesToDistribute, countryID);

    return put;
}


