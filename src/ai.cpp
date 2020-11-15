//
// Created by lukac on 11/6/2020.
//

/*
Overview of AI
 -calculateDistrictScoresForNextRound()
 -chooseDistrictsToHeal()



*/

#include "ai.h"

uint64_t fuckCpp[4] = {0};
Grid AI::grid2 = Grid(0, 0, fuckCpp);


void AI::calculateDistrictScoresForNextRound(size_t countryID, std::set<ScoreHolder> &districtScores) {
    if (grid2.getCurrentTick() == 1) {
        startFromGridBorder(countryID, districtScores);
    } else {
        std::vector<Point> startPoints;
        findBorder(countryID, startPoints);
        for (const auto &center:startPoints) {
            for (const auto &p:center.getNeighbours()) {
                District neighbourD = grid2.getDistrictByPoint(p);
                if (neighbourD.isClear()) {
                    continue;
                } else {
                    calculateScore(districtScores, neighbourD, countryID);
                }
            }
        }

    }
}

void AI::findBorder(size_t countryID, std::vector<Point> &result) {
    auto districts = grid2.getCountryByID(countryID).getAssignedDistricts();
    for (auto d:districts) {
        for (auto f:d->getAssignedFields()) {
            Point center = grid2.getPointByFieldID(f->getFieldID());
            for (const auto &p:center.getNeighbours()) {
                if (grid2.getDistrictByPoint(p) != grid2.getDistrictByPoint(center)) {
                    if (!grid2.getDistrictByPoint(p).isClear()) {
                        result.emplace_back(center);
                        break;
                    }
                }

            }
        }
    }
}

void AI::startFromGridBorder(size_t countryID, std::set<ScoreHolder> &districtScores) {
    std::vector<Point> border = addBorderFields();
    for (const auto &p:border) {
        District &district = grid2.getDistrictByPoint(p);
        calculateScore(districtScores, district, countryID);
    }
}

void AI::calculateScore(std::set<ScoreHolder> &districtScores, const District &district, size_t countryID) {
    if (!district.isClear()) {
        auto score = Utils::ScoreHolder(district.getDistrictID());
        int vaccinesNeededForTotalHealing = 0;
        for (auto fieldPointer:district.getAssignedFields()) {
            //assuming grid is 1 tick in the future, and no unhealed district in the country
            vaccinesNeededForTotalHealing += fieldPointer->vaccinesToPutForTotalHealing(countryID);
        }
        int changeInVaccines = grid2.calculateChangeInProducedVaccinesByHealingDistrict(countryID,
                                                                                        district); //todo: +aStarPathVaccineCost;
        //todo: store path to district (prob in scoreHolder as well?)
        score = Utils::ScoreHolder(changeInVaccines, vaccinesNeededForTotalHealing, district.getDistrictID());
        districtScores.insert(score);
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
                //numberOfVaccinesToDistribute += countryStoredVaccines - 1;
            }
        }
    }
    return back;
}

std::vector<VaccineData> AI::chooseFieldsToVaccinate(int numberOfVaccinesToDistribute, size_t countryID) {
    std::set<ScoreHolder> districtScores;
    std::vector<VaccineData> fieldsToHealSendBack;
    Grid *originalGrid = Logic::getGrid();

    //simulate next round
    Logic::setGrid(&AI::grid2);
    Logic::simulateTO(0, grid2.getCurrentTick() + 1, countryID);
    Logic::setGrid(originalGrid);

    //check if grid is clear
    grid2.updateClearByFieldCheck();
    if (grid2.isClear()) return std::vector<VaccineData>();

    //calculate district scores
    AI::calculateDistrictScoresForNextRound(countryID, districtScores);

    //calculate fields to heal
    modeB(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
    /*
    //ToDo Filter out districts which cannot be reached. -- this is done I think
    modeA(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
    if (fieldsToHealSendBack.empty()) {
        modeB(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
    }*/

    //TODO: refactor -- start points after first round
    if (originalGrid->getCurrentTick() == 0) {
        auto &district = originalGrid->getDistrictByPoint(fieldsToHealSendBack[0].getPoint());
        originalGrid->getCountryByID(countryID).addAssignedDistrict(&district);
    }
    return fieldsToHealSendBack;
}

//mode A: We check if we can heal entire districts in 1 turn, so our production capacity can increase
void AI::modeA(int &numberOfVaccinesToDistribute, size_t countryID, std::vector<ScoreHolder> &districtScores,
               std::vector<VaccineData> &fieldsToHealSendBack) {
    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::ProfIndex> orderedDistrictScores(
            districtScores.begin(), districtScores.end());
    while (!orderedDistrictScores.empty()) {
        ScoreHolder bestDistrict = orderedDistrictScores.top();
        if (numberOfVaccinesToDistribute > bestDistrict.getVaccinesNeededForHealing())
            addFieldsToHealWithFlood(numberOfVaccinesToDistribute, countryID, fieldsToHealSendBack, bestDistrict);
        orderedDistrictScores.pop();
    }
}


void AI::addFieldsToHealWithFlood(int &numberOfVaccinesToDistribute, size_t countryID,
                                  std::vector<VaccineData> &fieldsToHealSendBack,
                                  ScoreHolder maxScoredDistrict) {
    std::set<Field *> fieldsToHeal = grid2.getDistrictByID(maxScoredDistrict.getDistrictID()).getAssignedFields();
    Point startPoint = calculateStartPoint(fieldsToHeal, countryID);
    std::vector<Field *> fieldsToHealContinuous;
    floodDistrict(startPoint, fieldsToHeal, fieldsToHealContinuous);
    //ToDo check if fieldsToHeal is not empty --> nem volt folytonos a terület
    //check proposed by woranhun -- in really extreme cases it can make problem
    if (maxScoredDistrict.ChangeInVaccines() < 0) return;
    //get the fields of the district
    for (const auto &field:fieldsToHealContinuous) {
        int vaccinesToBeUsed = field->vaccinesToPutForTotalHealing(countryID);
        if (numberOfVaccinesToDistribute > vaccinesToBeUsed) {
            numberOfVaccinesToDistribute -= vaccinesToBeUsed;
            VaccineData vc = VaccineData(grid2.getCoordinatesByID(field->getFieldID()), vaccinesToBeUsed,
                                         countryID);
            //TODO: a-star algo modifies here too
            fieldsToHealSendBack.push_back(vc);
        }
    }

}

//mode B: we check which district is the easiest to heal, and try to heal that
void AI::modeB(int numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
               std::vector<VaccineData> &fieldsToHealSendBack) {
    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::ProfIndex> orderedDistrictScores(
            districtScores.begin(), districtScores.end());
    std::vector<Point> startPoints;

    //ToDO refactor orderedDistrictScores
    while (!orderedDistrictScores.empty() and fieldsToHealSendBack.empty()) {
        ScoreHolder topElement = orderedDistrictScores.top();
        if (grid2.getCurrentTick() == 1) {
            startPoints = addBorderFields(topElement.getDistrictID());
        } else {
            findBorder(countryID, startPoints);
        }
        addFieldsToHealWithDijsktra(numberOfVaccinesToDistribute, countryID, fieldsToHealSendBack,
                                    orderedDistrictScores.top(), startPoints);
        orderedDistrictScores.pop();
    }
}

Point AI::calculateStartPoint(const std::set<Field *> &fieldsToCalc, size_t countryID) {
    Grid *g = Logic::getGrid();
    for (const auto field:fieldsToCalc) {
        const Point &p = g->getPointByFieldID(field->getFieldID());
        if (g->getCountryByID(countryID).isNeighbourVaccinatedFields(p)) return p;
    }
    throw std::runtime_error("calculateStartPointFailed -- you tried to heal an invalid district");
    //return g->getCoordinatesByID((*fieldsToCalc.begin())->getFieldID());
}

std::vector<Point> AI::calculateStartPoints(const std::set<Field *> &fieldsToCalc, size_t countryID) {
    Grid *g = Logic::getGrid();
    std::vector<Point> goodStartPoints;
    for (const auto field:fieldsToCalc) {
        const Point &p = g->getPointByFieldID(field->getFieldID());
        if (g->getCountryByID(countryID).isNeighbourVaccinatedFields(p)) {
            goodStartPoints.push_back(p);
        }
    }
    return goodStartPoints;
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
AI::floodDistrict(const Point &p, std::set<Field *> &notVisitedFields, std::vector<Field *> &orderedFields) {
    if (!p.withinBounds())return;
    //Ez itt egy lamda függvény :(
    auto centerIter = std::find_if(notVisitedFields.begin(), notVisitedFields.end(), [p](Field *const &obj) {
        return grid2.getPointByFieldID(obj->getFieldID()) == p;
    });
    if (centerIter == notVisitedFields.end()) return;
    orderedFields.emplace_back(*centerIter);
    notVisitedFields.erase(centerIter);
    for (const auto &selected:p.getNeighbours()) {
        if (!selected.withinBounds())continue;
        if (grid2.getDistrictByPoint(p) == grid2.getDistrictByPoint(selected)) {
            floodDistrict(selected, notVisitedFields, orderedFields);
        }
    }
}

// Pontok között keresünk legrövidebb utat. Ezek alapján vakcinázunk. Meglátjuk jó lesz-e.
void AI::mikoltMedzsikIdea(const std::vector<Point> &startPoints, const std::set<Field *> &fieldsToHeal,
                           std::vector<Field *> &result, size_t countryID) {
    std::pair<std::vector<Point>, int> minPath({}, INT_MAX);
    for (const auto &p:startPoints) {
        Point end = grid2.getPointByFieldID((*fieldsToHeal.begin())->getFieldID());
        std::pair<std::vector<Point>, int> dijkstraResult;
        GraphAlgos ga;
        ga.dijkstra(p, end, dijkstraResult, countryID);
        if (dijkstraResult.second < minPath.second) minPath = dijkstraResult;
    }

}

void AI::addFieldsToHealWithDijsktra(int &numberOfVaccinesToDistribute, size_t countryID,
                                     std::vector<VaccineData> &fieldsToHealSendBack,
                                     ScoreHolder maxScoredDistrict, std::vector<Point> &startPoints) {
    District district = grid2.getDistrictByID(maxScoredDistrict.getDistrictID());
    std::vector<Point> endPoints;
    for (auto field:district.getAssignedFields()) {
        if (field->getCurrentInfectionRate() > 0) {
            endPoints.push_back(grid2.getCoordinatesByID(field->getFieldID()));
        }
    }
    GraphAlgos ga;
    std::pair<std::vector<Point>, int> result;
    std::pair<std::vector<Point>, int> minResult({Point(-1, -1)}, INT_MAX);
    Point startPForMinimum;
    for (const auto &endPoint:endPoints) {
        for (const auto &startPoint:startPoints) {
            if (startPoint == endPoint) {
                result = std::pair(std::vector(1, startPoint),
                                   grid2.getFieldByPoint(startPoint).vaccinesToPutMinimal(countryID));
            } else {
                result.first.push_back(startPoint);
                ga.dijkstra(startPoint, endPoint, result, countryID);
                result.second += grid2.getFieldByPoint(startPoint).vaccinesToPutMinimal(countryID);
            }
            if (minResult.second > result.second) {
                minResult = result;
                startPForMinimum = startPoint;
            }
        }
    }
    if (numberOfVaccinesToDistribute >= minResult.second) {
        for (const auto &p:minResult.first) {
            int vaccines = grid2.getFieldByPoint(p).vaccinesToPutMinimal(countryID);
            if (numberOfVaccinesToDistribute > vaccines) {
                fieldsToHealSendBack.emplace_back(VaccineData(p, vaccines, countryID));
                numberOfVaccinesToDistribute -= vaccines;
            } else {
                //todo: ez lehet fog majd gondot okozni
                return;
            }
        }
    }

}

std::vector<Point> AI::addBorderFields() {
    //ToDo ezt elég 1x kiszámolni
    std::vector<Point> border;
    for (size_t x = 1; x < grid2.getWidth() - 1; ++x) {

        border.emplace_back(0, x);
        border.emplace_back(grid2.getHeight() - 1, x);
    }
    for (size_t y = 0; y < grid2.getHeight(); ++y) {
        border.emplace_back(y, 0);
        border.emplace_back(y, grid2.getWidth() - 1);
    }
    return border;
}

std::vector<Point> AI::addBorderFields(size_t districtID) {
    //ToDo ezt elég 1x kiszámolni
    std::vector<Point> border;
    for (auto f :grid2.getDistrictByID(districtID).getAssignedFields()) {
        Point p = grid2.getPointByFieldID(f->getFieldID());
        if (p.isBorder())border.push_back(p);
    }
    return border;
}

