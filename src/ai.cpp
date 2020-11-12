//
// Created by lukac on 11/6/2020.
//

#include "ai.h"

//std::priority_queue<Utils::ScoreHolder> AI::districtScores = std::priority_queue<Utils::ScoreHolder>();
uint64_t fuckCpp[4] = {0};
Grid AI::grid2 = Grid(0, 0, fuckCpp);


void
AI::calculateDistrictScoresForNextRound(size_t countryID, std::vector<ScoreHolder> &districtScores) {
    // heavily unoptimised code for readability
    Grid *originalGrid = Logic::getGrid();
    Logic::setGrid(&AI::grid2);
    Logic::simulateTO(0, grid2.getCurrentTick() + 1, countryID);
    Logic::setGrid(originalGrid);
    if (originalGrid->getCurrentTick() == 0) {
        for (size_t x = 1; x < originalGrid->getWidth() - 1; ++x) {
            District &district = grid2.getDistrictByPoint(Point(0, x));
            CalculateScore(districtScores, district);

            District &district2 = grid2.getDistrictByPoint(Point(originalGrid->getHeight() - 1, x));
            CalculateScore(districtScores, district2);
        }
        for (size_t y = 0; y < originalGrid->getHeight(); ++y) {
            District &district = grid2.getDistrictByPoint(Point(y, 0));
            CalculateScore(districtScores, district);

            District &district2 = grid2.getDistrictByPoint(Point(y, originalGrid->getWidth() - 1));
            CalculateScore(districtScores, district2);
        }
    } else {
        for (size_t i = 0; i < grid2.numberOfDistricts(); ++i) {
            District &district = grid2.getDistrictByID(i);
            CalculateScore(districtScores, district);
        }
    }
}

void AI::CalculateScore(std::vector<ScoreHolder> &districtScores, const District &district) {
    if (!district.isClear()) {
        auto score = Utils::ScoreHolder(district.getDistrictID());
        int vaccinesNeededForTotalHealing = 0;
        for (Field *fieldPointer:district.getAssignedFields()) {
            //assuming grid is 1 tick in the future, and no unhealed district in the country
            vaccinesNeededForTotalHealing += std::max((int) std::ceil(
                    (fieldPointer->getCurrentInfectionRate() - fieldPointer->getVaccinationRate()) /
                    fieldPointer->getPopulationDensity()), 6 - fieldPointer->getPopulationDensity());
        }
        int changeInProducedVaccines = 0;
        int changeInDefenseVaccines = 0;
        for (Field *fieldPointer:district.getAssignedFields()) {
            calculateChangeByHealingField(fieldPointer, changeInProducedVaccines, changeInDefenseVaccines);
        }
        int changeInVaccines = changeInProducedVaccines - changeInDefenseVaccines;//todo: +aStarPathVaccineCost;
        //todo: store path to district (prob in scoreHolder as well?)
        score = Utils::ScoreHolder(changeInVaccines, vaccinesNeededForTotalHealing, district.getDistrictID());
        districtScores.push_back(score);
    }
}

void AI::calculateChangeByHealingField(const Field *fieldPointer, int &changeInProducedVaccines,
                                       int &changeInDefenseVaccines) {
    changeInProducedVaccines += 2;
    Point center = grid2.getCoordinatesByID(fieldPointer->getFieldID());
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
        if (!selected.withinBounds(Point(grid2.getHeight(),grid2.getWidth()))) continue;
        if (grid2.getDistrictByPoint(selected) == grid2.getDistrictByPoint(center)) continue;
        //todo: calculation assumes only 1 district healed/tick, this is wrong
        int plusMinus;
        if (grid2.getDistrictByPoint(selected).isClear()) plusMinus = -1; else plusMinus = 1;
        changeInDefenseVaccines += (int) (plusMinus * (ceil(
                (6 - grid2.getFieldByPoint(selected).getPopulationDensity()) / (double) 3)));
    }
}

std::vector<VaccineData> AI::chooseFieldsToVaccinate(int numberOfVaccinesToDistribute, size_t countryID) {
    std::vector<ScoreHolder> data;
    AI::calculateDistrictScoresForNextRound(countryID, data);
    //ToDo Filter out districts which cannot be reached.
    //ToDO A* to make a path to all districts
    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::ProfIndex> districtScores(data.begin(),
                                                                                                  data.end());
    std::vector<VaccineData> fieldsToHeal;
    while (!districtScores.empty()) {
        Utils::ScoreHolder maxScoredDistrict = districtScores.top();
        //check proposed by woranhun WARNING in extreme cases it can make problem
        if (maxScoredDistrict.getProfitabilityIndex() < 1) break;
        if (numberOfVaccinesToDistribute >= maxScoredDistrict.getVaccinesNeededForHealing()) {
            //get the fields of the district
            for (auto field:grid2.getDistrictByID(maxScoredDistrict.getDistrictID()).getAssignedFields()) {
                int vaccines = std::max((int) std::ceil(
                        (field->getCurrentInfectionRate() - field->getVaccinationRate()) /
                        field->getPopulationDensity()), 6 - field->getPopulationDensity());
                if (vaccines > 0) {
                    VaccineData vc = VaccineData(grid2.getCoordinatesByID(field->getFieldID()), vaccines, countryID);
                    //TODO: a-star algo modifies here too
                    fieldsToHeal.push_back(vc);
                }
            }
            numberOfVaccinesToDistribute -= districtScores.top().getVaccinesNeededForHealing();
        }
        if (numberOfVaccinesToDistribute == 0) break;
        districtScores.pop();
    }
    //TODO: might need to fieldsToHeal.pop_back(), if yes, don't forget to put the if loop after the profitability check
    //TODO: look after I have internet access, if it's passed on reference will it be fucked-up?
    if (!fieldsToHeal.empty()) return fieldsToHeal;
    //TODO: mode B - get a district and try to heal it
    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::TotalHealing> districtScores2(data.begin(),
                                                                                                      data.end());
    Utils::ScoreHolder maxScoredDistrict = districtScores2.top();
    for (auto field:grid2.getDistrictByID(maxScoredDistrict.getDistrictID()).getAssignedFields()) {
        int vaccines = std::max((int) std::ceil(
                (field->getCurrentInfectionRate() - field->getVaccinationRate()) /
                field->getPopulationDensity()), 6 - field->getPopulationDensity());
        if (vaccines > 0) {
            VaccineData vc = VaccineData(grid2.getCoordinatesByID(field->getFieldID()), vaccines, countryID);
            //TODO: a-star algo modifies here too
            fieldsToHeal.push_back(vc);
        }
    }
    std::vector<VaccineData> fieldsToHealOrdered;
    //ToDo make not hard coded
    Point fromWhere(28,22);
    floodDistrict(fromWhere,fieldsToHeal,fieldsToHealOrdered);
    //ToDo check if fieldsToHeal is not empty --> nem volt folytonos a terület
    return fieldsToHealOrdered;
}

std::vector<VaccineData> &
AI::calculateBackVaccines(std::vector<VaccineData> &back, int &numberOfVaccinesToDistribute, size_t countryID) {
    //ToDo We should take back vaccines only if the district is healed.
    //ToDO We should check only the cells where we put vaccines in the past.
    for (int y = 0; y < grid2.getHeight(); ++y) {
        for (int x = 0; x < grid2.getWidth(); ++x) {
            std::map<size_t, int> allStoredVaccines = grid2.getFieldByPoint(Point(y, x)).getStoredVaccines();
            int countryStoredVaccines;
            try { countryStoredVaccines = allStoredVaccines.at(countryID); }
            catch (std::out_of_range &exc) { countryStoredVaccines = 0; }
            //Egy területről az összes tartalék vakcinát nem lehet visszavenni, legalább 1 egységnyit ott kell hagyni.
            if (countryStoredVaccines > 1) {
                back.emplace_back(Point(y, x), countryStoredVaccines - 1, countryID);
                numberOfVaccinesToDistribute += countryStoredVaccines - 1;
            }
        }
    }
    return back;
}

std::vector<VaccineData> &
AI::calculatePutVaccines(std::vector<VaccineData> &put, int numberOfVaccinesToDistribute, size_t countryID) {
    put = chooseFieldsToVaccinate(numberOfVaccinesToDistribute, countryID);
    return put;
}
//floood kap egy Field tömböt + egy pontot, ezt olyan sorrandbe rendezni, hogy lerakható legyen.
void AI::floodDistrict(const Point& p, std::vector<VaccineData> &notVisitedFields, std::vector<VaccineData>& orderedFields) {
    if(!p.withinBounds(Point(grid2.getHeight(),grid2.getWidth())))return;
                                                                                    //Ez itt egy lamda függvény :(
    auto centerIter = std::find_if(notVisitedFields.begin(), notVisitedFields.end(), [p](VaccineData const& obj){
        return obj.getPoint() == p;
    } );
    if ( centerIter == notVisitedFields.end()) return;
    orderedFields.emplace_back(*centerIter);
    notVisitedFields.erase(centerIter);
    //Up
    if(grid2.getDistrictByPoint(p)==grid2.getDistrictByPoint(Point(p.getY()+1,p.getX())))
        floodDistrict(Point(p.getY()+1,p.getX()),notVisitedFields,orderedFields);
    //Right
    if(grid2.getDistrictByPoint(p)==grid2.getDistrictByPoint(Point(p.getY(),p.getX()+1)))
        floodDistrict(Point(p.getY(),p.getX()+1),notVisitedFields,orderedFields);
    //Down
    if(grid2.getDistrictByPoint(p)==grid2.getDistrictByPoint(Point(p.getY()-1,p.getX())))
        floodDistrict(Point(p.getY()-1,p.getX()),notVisitedFields,orderedFields);
    //Left
    if(grid2.getDistrictByPoint(p)==grid2.getDistrictByPoint(Point(p.getY(),p.getX()-1)))
        floodDistrict(Point(p.getY(),p.getX()-1),notVisitedFields,orderedFields);
}
