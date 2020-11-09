//
// Created by lukac on 11/6/2020.
//

#include "ai.h"

std::map<size_t, Utils::ScoreHolder> AI::districtScores = std::map<size_t, Utils::ScoreHolder>();
uint64_t fuckCpp[4] = {0};
Grid AI::grid = Grid(0, 0, fuckCpp);

void AI::calculateDistrictScoresForNextRound(size_t countryID) {
    // heavily unoptimised code for readability
    Grid *logicGrid = Logic::getGrid();
    Logic::setGrid(&AI::grid);
    AI::simulateTO(0, grid.getCurrentTick() + 1, countryID);
    Logic::setGrid(logicGrid);
    for (size_t i = 0; i < grid.numberOfDistricts(); ++i) {
        District &district = grid.getDistrictByID(i);
        Utils::ScoreHolder score = Utils::ScoreHolder();
        if (!district.isClear()) {
            int vaccinesNeededForTotalHealing = 0;
            for (Field *fieldPointer:district.getAssignedFields()) {
                //assuming grid is 1 tick in the future, and no unhealed district in the country
                vaccinesNeededForTotalHealing += std::ceil(
                        (fieldPointer->getCurrentInfectionRate() - fieldPointer->getVaccinationRate()) /
                        fieldPointer->getPopulationDensity());
            }
            int changeInProducedVaccines = 0;
            int changeInDefenseVaccines = 0;
            for (Field *fieldPointer:district.getAssignedFields()) {
                calculateChangeByHealingField(fieldPointer, changeInProducedVaccines, changeInDefenseVaccines);
            }
            int changeInVaccines = changeInProducedVaccines - changeInDefenseVaccines;//todo: +aStarPathVaccineCost;
            //todo: store path to district (prob in scoreholder as well?)
            score = Utils::ScoreHolder(changeInVaccines, vaccinesNeededForTotalHealing);
        }
        districtScores[district.getDistrictID()] = score;
    }
}

void AI::calculateChangeByHealingField(const Field *fieldPointer, int &changeInProducedVaccines,
                                       int &changeInDefenseVaccines) {
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
        if (selected.getX() < 0 || selected.getY() < 0 || selected.getX() > grid.getWidth() - 1 ||
            selected.getY() > grid.getHeight() - 1) {
            continue;
        }
        if (grid.getDistrictByPoint(selected) == grid.getDistrictByPoint(center)) continue;
        //todo: calculation assumes only 1 district healed/tick, this is wrong
        int plusMinus;
        if (grid.getDistrictByPoint(selected).isClear()) plusMinus = -1; else plusMinus = 1;
        changeInDefenseVaccines += (int) (plusMinus * (ceil(
                (6 - grid.getFieldByPoint(selected).getPopulationDensity()) / (double) 3)));
    }
}

std::vector<VaccineData> AI::chooseDistrictsToHeal(int numberOfVaccinesToDistribute, size_t countryID) {
    AI::districtScores.clear();
    AI::calculateDistrictScoresForNextRound(countryID);
    std::vector<VaccineData> districtsToHeal = std::vector<VaccineData>();
    while (!AI::districtScores.empty()) {
        size_t maxScoredDistrict = findBestDistrict();
        if (numberOfVaccinesToDistribute - AI::districtScores[maxScoredDistrict].getVaccinesNeededForHealing() >= 0) {
            //get the fields of the district
            for (auto field:grid.getDistrictByID(maxScoredDistrict).getAssignedFields()) {
                int vaccines = std::ceil(
                        (field->getCurrentInfectionRate() - field->getVaccinationRate()) /
                        field->getPopulationDensity());
                if (vaccines > 0) {
                    VaccineData vc = VaccineData(grid.getCoordinatesByID(field->getFieldID()), vaccines);
                    //TODO: a-star algo modifies here too
                    districtsToHeal.push_back(vc);
                }
            }
            numberOfVaccinesToDistribute -= AI::districtScores[maxScoredDistrict].getVaccinesNeededForHealing();
        }
        if (numberOfVaccinesToDistribute == 0) break;
        auto it = districtScores.find(maxScoredDistrict);
        districtScores.erase(it);
    }
    //TODO: might need to districtsToHeal.pop_back(), if yes, don't forget to put the if loop after the profitability check
    //TODO: look after I have internet access, if it's passed on reference will it be fucked-up?
    if (!districtsToHeal.empty()) return districtsToHeal;
    //TODO: mode B - get a district and try to heal it
    return districtsToHeal;
}

size_t AI::findBestDistrict() {
    size_t maxScoredDistrict = AI::districtScores.begin()->first;
    for (auto scores:districtScores) {
        //check proposed by woranhun WARNING in extreme cases it can make problem
        if (scores.second.getProfitabilityIndex() < 1) break;
        if (scores.second.getProfitabilityIndex() > districtScores[maxScoredDistrict].getProfitabilityIndex()) {
            maxScoredDistrict = scores.first;
        }
    }
    return maxScoredDistrict;
}

std::vector<VaccineData> &
AI::calculateBackVaccines(std::vector<VaccineData> &back, size_t tickID, int &numberOfVaccinesToDistribute,
                          size_t countryID) {
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
    return back;
}

std::vector<VaccineData> &
AI::calculatePutVaccines(std::vector<VaccineData> &put, size_t tickID, int numberOfVaccinesToDistribute,
                         size_t countryID) {
    //overwrite, bc you need to return the last values (thx Ericsson)
    put = chooseDistrictsToHeal(numberOfVaccinesToDistribute, countryID);
    return put;
}
