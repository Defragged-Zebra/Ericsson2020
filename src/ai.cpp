//
// Created by lukac on 11/6/2020.
//

#include "ai.h"

uint64_t fuckCpp[4] = {0};
Grid *AI::grid2 = new Grid(0, 0, fuckCpp);

void AI::copyGrid(Grid *originalGrid) {
    delete AI::grid2;
    AI::grid2 = new Grid(*originalGrid);
}

std::vector<VaccineData> &
AI::calculateBackVaccines(std::vector<VaccineData> &back, int &numberOfVaccinesToDistribute, size_t countryID) {
//    TODO: this has been commented out, because it caused errors (idk which one), but this should be implemented?
//    for (int y = 0; y < grid2->getHeight(); ++y) {
//        for (int x = 0; x < grid2->getWidth(); ++x) {
//            std::map<size_t, int> allStoredVaccines = grid2->getFieldByPoint(Point(y, x)).getStoredVaccines();
//            int countryStoredVaccines;
//            try { countryStoredVaccines = allStoredVaccines.at(countryID); }
//            catch (std::out_of_range &exc) { countryStoredVaccines = 0; }
//            //Egy területről az összes tartalék vakcinát nem lehet visszavenni, legalább 1 egységnyit ott kell hagyni.
//            if (countryStoredVaccines > 1) {
//                back.emplace_back(Point(y, x), countryStoredVaccines - 1, countryID);
//                //numberOfVaccinesToDistribute += countryStoredVaccines - 1;
//            }
//        }
//    }
    return back;
}

std::vector<VaccineData> &
AI::calculatePutVaccines(std::vector<VaccineData> &put, int &numberOfVaccinesToDistribute, size_t countryID) {
    //std::cerr<<"checkpoint 4.1"<<std::endl;
    std::vector<VaccineData> put2 = chooseFieldsToVaccinate(numberOfVaccinesToDistribute, countryID);
    Country &c = Logic::getGrid()->getCountryByID(countryID);
    //std::cerr<<"checkpoint 4.2"<<std::endl;
    std::vector<std::vector<bool>> check(grid2->getHeight());
    for (auto &row:check) {
        row = std::vector<bool>(grid2->getWidth(), true);
    }
    //std::cerr<<"checkpoint 4.3"<<std::endl;
    for (const auto &vd:put2)
        if (check[vd.getY()][vd.getX()]) {
            check[vd.getY()][vd.getX()] = false;
            c.addToVaccinatedFields(vd.getPoint(), AI::grid2->getDistrictByPoint(vd.getPoint()).getDistrictID());
            put.push_back(vd);
        }
    return put;
}

std::vector<VaccineData> AI::chooseFieldsToVaccinate(int &numberOfVaccinesToDistribute, size_t countryID) {
    //std::cerr<<"checkpoint 4.1.1"<<std::endl;
    std::set<ScoreHolder> districtScores;
    std::vector<VaccineData> fieldsToHealSendBack;
    Grid *originalGrid = Logic::getGrid();
    //std::cerr<<"checkpoint 4.1.2"<<std::endl;
    //simulate next round
    Logic::setGrid(AI::grid2);
    Logic::simulateTO(0, grid2->getCurrentTick() + 1, countryID);
    Logic::setGrid(originalGrid);
    //std::cerr<<"checkpoint 4.1.3"<<std::endl;
    //check if grid is clear
    grid2->updateClearByFieldCheck();
    if (grid2->isClear()) return std::vector<VaccineData>();

    AI::calculateDistrictScoresForNextRound(countryID, districtScores);
    //std::cerr<<"checkpoint 4.1.4"<<std::endl;
    //TODO: label A
    //calculate fields to heal
    if (grid2->getCurrentTick() < SWITCH_TICK) {
        modeB(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
    } else {
        modeA(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
    }

    //if we don't do this, a very sad runtime error happens
    if (fieldsToHealSendBack.empty()) {
        fieldsToHealSendBack.emplace_back(Point(0, 0), 0, countryID);
    }

    //save the smallest vaccine value
    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::TotalHealing>
            orderedDistrictScores(districtScores.begin(), districtScores.end());
    int smallestDistrictValue = orderedDistrictScores.top().getVaccinesNeededForHealing();

    //TODO: could be merged with label A?
    //TODO: shouldn't this be a separate function?
    while (numberOfVaccinesToDistribute > smallestDistrictValue) {
        districtScores.clear();
        calculateDistrictScoresWithWannabes(countryID, districtScores);
        if (districtScores.empty()) break;
        std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::TotalHealing>
                orderedDistrictScores2(districtScores.begin(), districtScores.end());
        smallestDistrictValue = orderedDistrictScores2.top().getVaccinesNeededForHealing();
        if (grid2->getCurrentTick() < SWITCH_TICK) {
            modeB(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
        } else {
            //TODO: Why is there a separate function for modeB(), but one for modeA()
            modeWanna(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
        }
    }
    return fieldsToHealSendBack;
}

void AI::calculateDistrictScoresForNextRound(size_t countryID, std::set<ScoreHolder> &districtScores) {
    if (!(grid2->getCountryByID(countryID).hasDistrict())) {
        startFromGridBorder(countryID, districtScores);
    } else {
        std::set<Point> startPoints = grid2->getCountryByID(countryID).getBorder();
        for (const auto &center:startPoints) {
            for (const auto &p:center.getNeighbours()) {
                District neighbourD = grid2->getDistrictByPoint(p);
                if (neighbourD.isClear()) {
                    continue;
                } else {
                    calculateScore(districtScores, neighbourD, countryID);
                }
            }
        }
    }
}

void AI::startFromGridBorder(size_t countryID, std::set<ScoreHolder> &districtScores) {
    std::vector<Point> border = mapAddBorderFields();
    for (const auto &p:border) {
        District &district = grid2->getDistrictByPoint(p);
        calculateScore(districtScores, district, countryID);
    }
}

std::vector<Point> AI::mapAddBorderFields() {
    //ToDo ezt elég 1x kiszámolni
    std::vector<Point> border;
    for (size_t x = 1; x < grid2->getWidth() - 1; ++x) {
        border.emplace_back(0, x);
        border.emplace_back(grid2->getHeight() - 1, x);
    }
    for (size_t y = 0; y < grid2->getHeight(); ++y) {
        border.emplace_back(y, 0);
        border.emplace_back(y, grid2->getWidth() - 1);
    }
    return border;
}

//Score calculation: grid is 1 tick in the future, all districts of the country are clear
void AI::calculateScore(std::set<ScoreHolder> &districtScores, const District &district, size_t countryID) {
    if (!district.isClear()) {
        auto score = ScoreHolder(district.getDistrictID());
        int vaccinesNeededForTotalHealing = 0;
        for (auto fieldPointer:district.getAssignedFields()) {
            //TODO: if we can test, change this
            //if (!fieldPointer->isClear()) {
            //    vaccinesNeededForTotalHealing += fieldPointer->vaccinesToPutForTotalHealing(countryID);
            // or with dijkstra min value
            //}
            vaccinesNeededForTotalHealing += fieldPointer->vaccinesToPutForTotalHealing(countryID);
        }
        int changeInVaccines = grid2->calculateDistrictProductionCapacity(countryID, district);
        score = Utils::ScoreHolder(changeInVaccines, vaccinesNeededForTotalHealing, district.getDistrictID());
        districtScores.insert(score);
    }
}

//mode B: we check which district is the easiest to heal, and try to heal that
void AI::modeB(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
               std::vector<VaccineData> &fieldsToHealSendBack) {

    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::ProducedVaccines> orderedDistrictScores(
            districtScores.begin(), districtScores.end());
    std::vector<Point> startPoints;

    if ((grid2->getCountryByID(countryID).hasDistrict())) {
        //start from country's border
        while (!orderedDistrictScores.empty() and
               (fieldsToHealSendBack.empty() or (numberOfVaccinesToDistribute > 0))) {
            ScoreHolder maxScoredDistrict = orderedDistrictScores.top();
            //todo: delete if() later
            if (!(grid2->getCountryByID(countryID).hasDistrict())) {
                startPoints = addMapEdge(orderedDistrictScores.top().getDistrictID());
            } else {
                if (startPoints.empty()) {
                    std::set<Point> calcBorder = grid2->getCountryByID(countryID).getBorder();
                    startPoints = std::vector<Point>(calcBorder.begin(), calcBorder.end());
                }
            }
            addFieldsToHealWithDijsktra(numberOfVaccinesToDistribute, countryID, fieldsToHealSendBack,
                                        maxScoredDistrict, startPoints);
            grid2->getCountryByID(countryID).addWannabeDistrict(maxScoredDistrict.getDistrictID());
            orderedDistrictScores.pop();
        }
    } else {
        //start from edge of the grid
        while (!orderedDistrictScores.empty() and fieldsToHealSendBack.empty()) {
            ScoreHolder topElement = orderedDistrictScores.top();
            //todo: delete if() later
            if (!(grid2->getCountryByID(countryID).hasDistrict())) {
                startPoints = addMapEdge(orderedDistrictScores.top().getDistrictID());
            } else {
                std::set<Point> calcBorder = grid2->getCountryByID(countryID).getBorder();
                startPoints = std::vector<Point>(calcBorder.begin(), calcBorder.end());
            }
            addFieldsToHealWithDijsktra(numberOfVaccinesToDistribute, countryID, fieldsToHealSendBack,
                                        topElement, startPoints);
            orderedDistrictScores.pop();
        }
    }
}

std::vector<Point> AI::addMapEdge(size_t districtID) {
    //ToDo ezt elég 1x kiszámolni
    std::vector<Point> border;
    for (auto f :grid2->getDistrictByID(districtID).getAssignedFields()) {
        Point p = grid2->getPointByFieldID(f->getFieldID());
        if (p.isBorder())border.push_back(p);
    }
    return border;
}

void AI::addFieldsToHealWithDijsktra(int &numberOfVaccinesToDistribute, size_t countryID,
                                     std::vector<VaccineData> &fieldsToHealSendBack,
                                     const ScoreHolder &maxScoredDistrict, std::vector<Point> &startPoints) {
    District district = grid2->getDistrictByID(maxScoredDistrict.getDistrictID());
    std::vector<Point> endPoints;
    for (auto field:district.getAssignedFields()) {
        if (field->getCurrentInfectionRate() > 0) {
            endPoints.push_back(grid2->getCoordinatesByID(field->getFieldID()));
        }
    }
    GraphAlgos ga;
    std::pair<std::vector<Point>, int> result;
    Point startPForMinimum;
    for (const auto &endPoint:endPoints) {
        std::pair<std::vector<Point>, int> minResult({Point(-1, -1)}, INT_MAX);
        for (const auto &startPoint:startPoints) {
            //for optimization purposes
            if (startPoint == endPoint) {
                //works without template specification: https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
                //TODO: change to this
                //result = std::pair(std::vector(1, startPoint),
                //                   grid2->getFieldByPoint(startPoint).vaccinesToPutForTotalHealing(countryID));
                result = std::pair(std::vector(1, startPoint),
                                   grid2->getFieldByPoint(startPoint).vaccinesToPutMinimal(countryID));
            } else {
                //ga.dijkstra(startPoint, endPoint, result, countryID);
                ga.Astar(startPoint, endPoint, result, countryID);
                //ga.dijkstraLEGACY(startPoint, endPoint, result, countryID);
                result.first.insert(result.first.begin(), 1, startPoint);
                result.second += grid2->getFieldByPoint(startPoint).vaccinesToPutMinimal(countryID);
            }
            //update minimum
            if (minResult.second > result.second) {
                minResult = result;
                startPForMinimum = startPoint;
            }
        }
        //we want a path to each endpoint
        if (minResult.second > numberOfVaccinesToDistribute) return;
        for (const auto &p:minResult.first) {
            //TODO: top prio:
            //TODO: the put should omit it too, if we've already put vaccines to that field in this round -- this would solve the sometimes duplicates error below too
            if (!grid2->getDistrictByPoint(p).isClear() or
                (grid2->getFieldByPoint(p).getStoredVaccines()[countryID] > 0)) {
                int vaccines = grid2->getFieldByPoint(p).vaccinesToPutMinimal(countryID);
                //TODO: this "if" is not needed (see check above)
                if (numberOfVaccinesToDistribute >= vaccines) {
                    fieldsToHealSendBack.emplace_back(p, vaccines, countryID);
                    numberOfVaccinesToDistribute -= vaccines;
                    //sometimes duplicates -- refactor, and store this in country property
                    startPoints.emplace_back(p);
                } else {
                    return;
                }
            }
        }
    }
}

//mode A: We check if we can heal entire districts in 1 turn, so our production capacity can increase
void AI::modeA(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
               std::vector<VaccineData> &fieldsToHealSendBack) {

    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::ProducedVaccines>
            orderedDistrictScores(districtScores.begin(), districtScores.end());
    while (!orderedDistrictScores.empty()) {
        ScoreHolder maxScoredDistrict = orderedDistrictScores.top();
        if (numberOfVaccinesToDistribute > maxScoredDistrict.getVaccinesNeededForHealing()) {
            grid2->getCountryByID(countryID).addWannabeDistrict(maxScoredDistrict.getDistrictID());
            addFieldsToHealWithFlood(numberOfVaccinesToDistribute, countryID, fieldsToHealSendBack, maxScoredDistrict);
        }
        orderedDistrictScores.pop();
    }
}

void AI::addFieldsToHealWithFlood(int &numberOfVaccinesToDistribute, size_t countryID,
                                  std::vector<VaccineData> &fieldsToHealSendBack,
                                  ScoreHolder maxScoredDistrict) {
    std::set<Field *> fieldsToHeal = grid2->getDistrictByID(maxScoredDistrict.getDistrictID()).getAssignedFields();
    Point startPoint = calculateStartPointForFlood(fieldsToHeal, countryID);
    std::vector<Field *> fieldsToHealContinuous;
    floodDistrict(startPoint, fieldsToHeal, fieldsToHealContinuous);
    //check proposed by woranhun -- in really extreme cases it can make problem
    if (maxScoredDistrict.getChangeInVaccines() < 0) return;
    for (const auto &field:fieldsToHealContinuous) {
        int vaccinesToBeUsed = field->vaccinesToPutForTotalHealing(countryID);
        if (numberOfVaccinesToDistribute > vaccinesToBeUsed) {
            numberOfVaccinesToDistribute -= vaccinesToBeUsed;
            VaccineData vc = VaccineData(grid2->getCoordinatesByID(field->getFieldID()), vaccinesToBeUsed, countryID);
            fieldsToHealSendBack.push_back(vc);
        }
    }

}

Point AI::calculateStartPointForFlood(const std::set<Field *> &fieldsToCalc, size_t countryID) {
    Grid *g = Logic::getGrid();
    for (const auto field:fieldsToCalc) {
        const Point &p = g->getPointByFieldID(field->getFieldID());
        if (g->getCountryByID(countryID).isNeighbourToVaccinatedField(p)) return p;
    }
    throw std::runtime_error("calculateStartPointFailed -- you tried to heal an invalid area");
}

//flood kap egy Field tömböt + egy pontot, ezt olyan sorrandbe rendezni, hogy lerakható legyen.
void AI::floodDistrict(const Point &p, std::set<Field *> &notVisitedFields, std::vector<Field *> &orderedFields) {
    if (!p.withinBounds())return;
    //this is a lambda function :(
    auto centerIter = std::find_if(notVisitedFields.begin(), notVisitedFields.end(), [p](Field *const &obj) {
        return grid2->getPointByFieldID(obj->getFieldID()) == p;
    });
    if (centerIter == notVisitedFields.end()) return;
    orderedFields.emplace_back(*centerIter);
    notVisitedFields.erase(centerIter);
    for (const auto &selected:p.getNeighbours()) {
        if (!selected.withinBounds())continue;
        if (grid2->getDistrictByPoint(p) == grid2->getDistrictByPoint(selected)) {
            floodDistrict(selected, notVisitedFields, orderedFields);
        }
    }
}

void AI::calculateDistrictScoresWithWannabes(size_t countryID, std::set<ScoreHolder> &districtScores) {
    if (!(grid2->getCountryByID(countryID).hasDistrict())) {
        startFromGridBorder(countryID, districtScores);
    } else {
        std::set<Point> startPoints;
        calculateWannabeBorder(countryID);
        startPoints = grid2->getCountryByID(countryID).getWannabeBorder();

        auto wannabeDistricts = grid2->getCountryByID(countryID).getWannabeDistricts();
        for (const auto &center:startPoints) {
            for (const auto &p:center.getNeighbours()) {
                District neighbourD = grid2->getDistrictByPoint(p);
                if (neighbourD.isClear()) {
                    continue;
                } else if (std::find(wannabeDistricts.begin(), wannabeDistricts.end(),
                                     grid2->getDistrictByPoint(p).getDistrictID()) != wannabeDistricts.end()) {
                    continue;
                } else {
                    calculateScore(districtScores, neighbourD, countryID);
                }
            }
        }

    }
}

//mode Wanna: We check if we can heal entire districts in 1 turn, so our production capacity can increase
void AI::modeWanna(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
                   std::vector<VaccineData> &fieldsToHealSendBack) {

    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::ProducedVaccines>
            orderedDistrictScores(districtScores.begin(), districtScores.end());
    while (!orderedDistrictScores.empty()) {
        ScoreHolder maxScoredDistrict = orderedDistrictScores.top();
        if (numberOfVaccinesToDistribute > maxScoredDistrict.getVaccinesNeededForHealing()) {
            addFieldsToWannaHealWithFlood(numberOfVaccinesToDistribute, countryID, fieldsToHealSendBack,
                                          maxScoredDistrict);
            grid2->getCountryByID(countryID).addWannabeDistrict(maxScoredDistrict.getDistrictID());
            AI::calculateWannabeBorder(countryID);
        }
        orderedDistrictScores.pop();
    }
    for (const auto &vc:fieldsToHealSendBack) {
        grid2->getCountryByID(countryID).addWannabeDistrict(grid2->getDistrictByPoint(vc.getPoint()).getDistrictID());
    }
}


void AI::addFieldsToWannaHealWithFlood(int &numberOfVaccinesToDistribute, size_t countryID,
                                       std::vector<VaccineData> &fieldsToHealSendBack,
                                       ScoreHolder maxScoredDistrict) {
    std::set<Field *> fieldsToHeal = grid2->getDistrictByID(maxScoredDistrict.getDistrictID()).getAssignedFields();
    Point startPoint = calculateWannabeStartPoint(fieldsToHeal, countryID);
    std::vector<Field *> fieldsToHealContinuous;
    floodDistrict(startPoint, fieldsToHeal, fieldsToHealContinuous);
    //check proposed by woranhun -- in really extreme cases it can make problem
    if (maxScoredDistrict.getChangeInVaccines() < 0) return;
    //get the fields of the district
    for (const auto &field:fieldsToHealContinuous) {
        int vaccinesToBeUsed = field->vaccinesToPutForTotalHealing(countryID);
        if (numberOfVaccinesToDistribute > vaccinesToBeUsed) {
            numberOfVaccinesToDistribute -= vaccinesToBeUsed;
            VaccineData vc = VaccineData(grid2->getCoordinatesByID(field->getFieldID()), vaccinesToBeUsed, countryID);
            fieldsToHealSendBack.push_back(vc);
        }
    }

}


Point AI::calculateWannabeStartPoint(const std::set<Field *> &fieldsToCalc, size_t countryID) {
    for (const auto field:fieldsToCalc) {
        const Point &p = grid2->getPointByFieldID(field->getFieldID());
        if (grid2->getCountryByID(countryID).isNeighbourToVaccinatedField(p)) return p;
        if (grid2->getCountryByID(countryID).isNeighbourToWannabeVaccinatedField(p)) return p;
    }
    throw std::runtime_error("calculateWannabeStartPoint -- you tried to heal an invalid area");
}

void AI::calculateWannabeBorder(size_t countryID) {
    std::set<Point> futureBorder;
    Country &country = grid2->getCountryByID(countryID);
    const std::set<size_t> &wannabeDistricts = country.getWannabeDistricts();
    for (auto d:country.getWannabeDistricts()) {
        for (auto f:grid2->getDistrictByID(d).getAssignedFields()) {
            auto center = Point(f->getFieldID());
            for (const auto &p:center.getNeighbours()) {
                if (grid2->getDistrictByPoint(p) != grid2->getDistrictByPoint(center)) {
                    if (std::find(wannabeDistricts.begin(), wannabeDistricts.end(),
                                  grid2->getDistrictByPoint(p).getDistrictID()) == wannabeDistricts.end()) {
                        const std::set<Point> &border = country.getBorder();
                        if (std::find(border.begin(), border.end(), p) == border.end())
                            futureBorder.insert(center);
                        break;
                    }
                }
            }
        }
    }
    country.setWannabeBorder(futureBorder);
}
