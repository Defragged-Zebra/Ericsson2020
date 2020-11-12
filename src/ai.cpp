//
// Created by lukac on 11/6/2020.
//

#include "ai.h"

//std::priority_queue<Utils::ScoreHolder> AI::districtScores = std::priority_queue<Utils::ScoreHolder>();
uint64_t fuckCpp[4] = {0};
Grid AI::grid2 = Grid(0, 0, fuckCpp);


void
AI::calculateDistrictScoresForNextRound(size_t countryID, std::vector<ScoreHolder> &districtScores) {
    if (grid2.getCurrentTick() == 1) {
        for (size_t x = 1; x < grid2.getWidth() - 1; ++x) {
            District &district = grid2.getDistrictByPoint(Point(0, x));
            calculateScore(districtScores, district, countryID);

            District &district2 = grid2.getDistrictByPoint(Point(grid2.getHeight() - 1, x));
            calculateScore(districtScores, district2, countryID);
        }
        for (size_t y = 0; y < grid2.getHeight(); ++y) {
            District &district = grid2.getDistrictByPoint(Point(y, 0));
            calculateScore(districtScores, district, countryID);

            District &district2 = grid2.getDistrictByPoint(Point(y, grid2.getWidth() - 1));
            calculateScore(districtScores, district2, countryID);
        }
    } else {
        std::set<District *> districts = grid2.getCountryByID(countryID).getAssignedDistrictIDs();
        for (auto &d:districts) {
            for (auto &neighbourD:d->getNeighbourDistricts()) {
                calculateScore(districtScores, grid2.getDistrictByID(neighbourD), countryID);
            }
        }
    }
}

void AI::calculateScore(std::vector<ScoreHolder> &districtScores, const District &district, size_t countryID) {
    if (!district.isClear()) {
        auto score = Utils::ScoreHolder(district.getDistrictID());
        int vaccinesNeededForTotalHealing = 0;
        for (Field *fieldPointer:district.getAssignedFields()) {
            //assuming grid is 1 tick in the future, and no unhealed district in the country
            vaccinesNeededForTotalHealing += std::max((int) std::ceil(
                    (fieldPointer->getCurrentInfectionRate() - fieldPointer->getVaccinationRate()) /
                    fieldPointer->getPopulationDensity()), 6 - fieldPointer->getPopulationDensity());
        }
        int changeInVaccines = grid2.calculateChangeInProducedVaccinesByHealingDistrict(countryID,
                                                                                        district);//todo: +aStarPathVaccineCost;
        //todo: store path to district (prob in scoreHolder as well?)
        score = Utils::ScoreHolder(changeInVaccines, vaccinesNeededForTotalHealing, district.getDistrictID());
        districtScores.push_back(score);
    }
}


std::vector<VaccineData> &
AI::calculateBackVaccines(std::vector<VaccineData> &back, int &numberOfVaccinesToDistribute, size_t countryID) {
    //TODO: ezt az egeszet itt refactorolni kell majd
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

std::vector<VaccineData> AI::chooseFieldsToVaccinate(int numberOfVaccinesToDistribute, size_t countryID) {
    std::vector<ScoreHolder> data;
    Grid *originalGrid = Logic::getGrid();
    Logic::setGrid(&AI::grid2);
    Logic::simulateTO(0, grid2.getCurrentTick() + 1, countryID);
    Logic::setGrid(originalGrid);
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
    if (fieldsToHeal.empty()) {
        //mode B - get the easiest district and try to heal it
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
    }
    if (originalGrid->getCurrentTick() == 0) {
        District &district = originalGrid->getDistrictByPoint(fieldsToHeal.begin()->getPoint());
        originalGrid->getCountryByID(countryID).addAssignedDistrictID(&district);
    }

    Point fromWhere = AI::calculateStartPoint(fieldsToHeal, countryID);
    std::vector<VaccineData> fieldsToHealOrdered;
    floodDistrict(fromWhere, fieldsToHeal, fieldsToHealOrdered);
    //ToDo check if fieldsToHeal is not empty --> nem volt folytonos a terület
    return fieldsToHealOrdered;
}

Point AI::calculateStartPoint(std::vector<VaccineData> &fieldsToHeal, size_t countryID) {
    Grid *g = Logic::getGrid();
    for (const auto &vaccineData:fieldsToHeal) {
        const Point &p = vaccineData.getPoint();
        if (g->getCountryByID(countryID).isNeighbourVaccinatedFields(p)) return p;
    }
    throw std::runtime_error("calculateStartPointFailed");
}


std::vector<VaccineData> &
AI::calculatePutVaccines(std::vector<VaccineData> &put, int numberOfVaccinesToDistribute, size_t countryID) {
    put = chooseFieldsToVaccinate(numberOfVaccinesToDistribute, countryID);
    Country &c = Logic::getGrid()->getCountryByID(countryID);
    for (const auto &vd:put) c.addToVaccinatedFields(vd.getPoint());
    return put;
}

//floood kap egy Field tömböt + egy pontot, ezt olyan sorrandbe rendezni, hogy lerakható legyen.
void
AI::floodDistrict(const Point &p, std::vector<VaccineData> &notVisitedFields, std::vector<VaccineData> &orderedFields) {
    if (!p.withinBounds())return;
    //Ez itt egy lamda függvény :(
    auto centerIter = std::find_if(notVisitedFields.begin(), notVisitedFields.end(), [p](VaccineData const &obj) {
        return obj.getPoint() == p;
    });
    if (centerIter == notVisitedFields.end()) return;
    orderedFields.emplace_back(*centerIter);
    notVisitedFields.erase(centerIter);
    //Up
    if (grid2.getDistrictByPoint(p) == grid2.getDistrictByPoint(Point(p.getY() + 1, p.getX())))
        floodDistrict(Point(p.getY() + 1, p.getX()), notVisitedFields, orderedFields);
    //Right
    if (grid2.getDistrictByPoint(p) == grid2.getDistrictByPoint(Point(p.getY(), p.getX() + 1)))
        floodDistrict(Point(p.getY(), p.getX() + 1), notVisitedFields, orderedFields);
    //Down
    if (grid2.getDistrictByPoint(p) == grid2.getDistrictByPoint(Point(p.getY() - 1, p.getX())))
        floodDistrict(Point(p.getY() - 1, p.getX()), notVisitedFields, orderedFields);
    //Left
    if (grid2.getDistrictByPoint(p) == grid2.getDistrictByPoint(Point(p.getY(), p.getX() - 1)))
        floodDistrict(Point(p.getY(), p.getX() - 1), notVisitedFields, orderedFields);
}

