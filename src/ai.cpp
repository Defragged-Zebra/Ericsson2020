//
// Created by lukac on 11/6/2020.
//

#include "ai.h"

uint64_t fuckCpp[4] = {0};
Grid *AI::grid2 = new Grid(0, 0, fuckCpp);


std::vector<VaccineData> &
AI::calculateBackVaccines(std::vector<VaccineData> &back, int &numberOfVaccinesToDistribute, size_t countryID) {
    //TODO: ezt az egeszet itt refactorolni kell majd
    //ToDo We should take back vaccines only if the district is healed.
    //ToDO We should check only the cells where we put vaccines in the past.
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
    std::vector<VaccineData> put2 = chooseFieldsToVaccinate(numberOfVaccinesToDistribute, countryID);
    Country &c = Logic::getGrid()->getCountryByID(countryID);
    std::vector<std::vector<bool>> check(grid2->getHeight());
    for (auto &row:check) {
        row = std::vector<bool>(grid2->getWidth(), true);
    }

    for (const auto &vd:put2)
        if (check[vd.getY()][vd.getX()]) {
            check[vd.getY()][vd.getX()] = false;
            c.addToVaccinatedFields(vd.getPoint(), AI::grid2->getDistrictByPoint(vd.getPoint()).getDistrictID());
            put.push_back(vd);
        }
    return put;
}


std::vector<VaccineData> AI::chooseFieldsToVaccinate(int &numberOfVaccinesToDistribute, size_t countryID) {
    std::set<ScoreHolder> districtScores;
    std::vector<VaccineData> fieldsToHealSendBack;
    Grid *originalGrid = Logic::getGrid();

    //simulate next round
    Logic::setGrid(AI::grid2);
    Logic::simulateTO(0, grid2->getCurrentTick() + 1, countryID);
    Logic::setGrid(originalGrid);

    //check if grid is clear
    grid2->updateClearByFieldCheck();
    if (grid2->isClear()) return std::vector<VaccineData>();

    //calculate district scores
    AI::calculateDistrictScoresForNextRound(countryID, districtScores);

    //save the smallest vaccine value
    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::TotalHealing> orderedDistrictScores(
            districtScores.begin(), districtScores.end());
    int smallestDistrictValue = orderedDistrictScores.top().getVaccinesNeededForHealing();

    //calculate fields to heal
    if (grid2->getCurrentTick() < SWITCH_TICK) {
        modeB(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
    } else {
        modeA(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
    }
    if (fieldsToHealSendBack.empty()) {
        modeC(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
    }

    while (numberOfVaccinesToDistribute > smallestDistrictValue) {
        districtScores.clear();
        calculateDistrictScoresWithWannabes(countryID, districtScores);
        if (districtScores.empty())break;
        std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::TotalHealing> orderedDistrictScores2(
                districtScores.begin(), districtScores.end());
        smallestDistrictValue = orderedDistrictScores2.top().getVaccinesNeededForHealing();
        if (grid2->getCurrentTick() < SWITCH_TICK) {
            modeB(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
        } else {
            modeWanna(numberOfVaccinesToDistribute, countryID, districtScores, fieldsToHealSendBack);
        }
    }

    return fieldsToHealSendBack;
}


void AI::calculateDistrictScoresForNextRound(size_t countryID, std::set<ScoreHolder> &districtScores) {
    if (!(grid2->getCountryByID(countryID).hasDistrict())) {
        startFromGridBorder(countryID, districtScores);
    } else {
        std::set<Point> startPoints;
        startPoints = grid2->getCountryByID(countryID).getBorder();

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

void AI::startFromGridBorder(size_t countryID, std::set<ScoreHolder> &districtScores) {
    std::vector<Point> border = mapAddBorderFields();

    for (const auto &p:border) {
        District &district = grid2->getDistrictByPoint(p);
        calculateScore(districtScores, district, countryID);
    }
}

void AI::calculateScore(std::set<ScoreHolder> &districtScores, const District &district, size_t countryID) {
    if (!district.isClear()) {
        auto score = Utils::ScoreHolder(district.getDistrictID());
        int vaccinesNeededForTotalHealing = 0;
        for (auto fieldPointer:district.getAssignedFields()) {
            //assuming grid is 1 tick in the future, and no unhealed district in the country
            //TODO: if field is clear, than field cost should be 0 or dijkstra
            vaccinesNeededForTotalHealing += fieldPointer->vaccinesToPutForTotalHealing(countryID);
        }
        int changeInVaccines = grid2->calculateDistrictProductionCapacity(countryID, district);

        score = Utils::ScoreHolder(changeInVaccines, vaccinesNeededForTotalHealing, district.getDistrictID());
        districtScores.insert(score);
    }
}


//mode A: We check if we can heal entire districts in 1 turn, so our production capacity can increase
void AI::modeA(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
               std::vector<VaccineData> &fieldsToHealSendBack) {
    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::ProducedVaccines> orderedDistrictScores(
            districtScores.begin(), districtScores.end());
    while (!orderedDistrictScores.empty()) {
        ScoreHolder maxScoredDistrict = orderedDistrictScores.top();
        if (numberOfVaccinesToDistribute > maxScoredDistrict.getVaccinesNeededForHealing()) {
            grid2->getCountryByID(countryID).addWannabeDistrict(maxScoredDistrict.getDistrictID());
            addFieldsToHealWithFlood(numberOfVaccinesToDistribute, countryID, fieldsToHealSendBack, maxScoredDistrict);
        }
        orderedDistrictScores.pop();
    }
}

//mode Wanna: We check if we can heal entire districts in 1 turn, so our production capacity can increase
void AI::modeWanna(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
                   std::vector<VaccineData> &fieldsToHealSendBack) {
    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::ProducedVaccines> orderedDistrictScores(
            districtScores.begin(), districtScores.end());
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


void AI::addFieldsToHealWithFlood(int &numberOfVaccinesToDistribute, size_t countryID,
                                  std::vector<VaccineData> &fieldsToHealSendBack,
                                  ScoreHolder maxScoredDistrict) {
    std::set<Field *> fieldsToHeal = grid2->getDistrictByID(maxScoredDistrict.getDistrictID()).getAssignedFields();
    Point startPoint = calculateStartPoint(fieldsToHeal, countryID);
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

//mode B: we check which district is the easiest to heal, and try to heal that
void AI::modeB(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
               std::vector<VaccineData> &fieldsToHealSendBack) {
    std::priority_queue<ScoreHolder, std::vector<ScoreHolder>, Compare::ProducedVaccines> orderedDistrictScores(
            districtScores.begin(), districtScores.end());
    std::vector<Point> startPoints;

    if ((grid2->getCountryByID(countryID).hasDistrict())) {
        while (!orderedDistrictScores.empty() and
               (fieldsToHealSendBack.empty() or (numberOfVaccinesToDistribute > 0))) {
            ScoreHolder maxScoredDisrtict = orderedDistrictScores.top();
            if (!(grid2->getCountryByID(countryID).hasDistrict())) {
                startPoints = mapAddBorderFieldsForDistrict(orderedDistrictScores.top().getDistrictID());
            } else {
                if (startPoints.empty()) {
                    std::set<Point> calcBorder = grid2->getCountryByID(countryID).getBorder();
                    startPoints = std::vector<Point>(calcBorder.begin(), calcBorder.end());
                }
            }
            addFieldsToHealWithDijsktra(numberOfVaccinesToDistribute, countryID, fieldsToHealSendBack,
                                        maxScoredDisrtict, startPoints);
            grid2->getCountryByID(countryID).addWannabeDistrict(maxScoredDisrtict.getDistrictID());
            orderedDistrictScores.pop();
        }
    } else {
        while (!orderedDistrictScores.empty() and fieldsToHealSendBack.empty()) {
            ScoreHolder topElement = orderedDistrictScores.top();
            if (!(grid2->getCountryByID(countryID).hasDistrict())) {
                startPoints = mapAddBorderFieldsForDistrict(orderedDistrictScores.top().getDistrictID());
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

void AI::modeC(int &numberOfVaccinesToDistribute, size_t countryID, std::set<ScoreHolder> &districtScores,
               std::vector<VaccineData> &fieldsToHealSendBack) {
    fieldsToHealSendBack.emplace_back(Point(0, 0), 0, countryID);
}


Point AI::calculateStartPoint(const std::set<Field *> &fieldsToCalc, size_t countryID) {
    Grid *g = Logic::getGrid();
    for (const auto field:fieldsToCalc) {
        const Point &p = g->getPointByFieldID(field->getFieldID());
        if (g->getCountryByID(countryID).isNeighbourToVaccinatedField(p)) return p;
    }
    throw std::runtime_error("calculateStartPointFailed -- you tried to heal an invalid area");
}

Point AI::calculateWannabeStartPoint(const std::set<Field *> &fieldsToCalc, size_t countryID) {
    for (const auto field:fieldsToCalc) {
        const Point &p = grid2->getPointByFieldID(field->getFieldID());
        if (grid2->getCountryByID(countryID).isNeighbourToVaccinatedField(p)) return p;
        if (grid2->getCountryByID(countryID).isNeighbourToWannabeVaccinatedField(p)) return p;
    }
    throw std::runtime_error("calculateWannabeStartPoint -- you tried to heal an invalid area");
}



//flood kap egy Field tömböt + egy pontot, ezt olyan sorrandbe rendezni, hogy lerakható legyen.
void AI::floodDistrict(const Point &p, std::set<Field *> &notVisitedFields, std::vector<Field *> &orderedFields) {
    if (!p.withinBounds())return;
    //Ez itt egy lamda függvény :(
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
            if (startPoint == endPoint) {
                //TODO: this if part is not needed, and result should be total healing btw
                result = std::pair(std::vector(1, startPoint),
                                   grid2->getFieldByPoint(startPoint).vaccinesToPutMinimal(countryID));
            } else {
                ga.dijkstra(startPoint, endPoint, result, countryID);
                result.first.insert(result.first.begin(), 1, startPoint);
                result.second += grid2->getFieldByPoint(startPoint).vaccinesToPutMinimal(countryID);
            }
            if (minResult.second > result.second) {
                minResult = result;
                startPForMinimum = startPoint;
            }
        }
        //we want a path to each endpoint
        if (minResult.second > numberOfVaccinesToDistribute) return;
        for (const auto &p:minResult.first) {
            //the put should omit it too, if we've already put vaccines to that field in this round -- this would solve the sometimes duplicates error below too
            if (!grid2->getDistrictByPoint(p).isClear() or
                (grid2->getFieldByPoint(p).getStoredVaccines()[countryID] > 0)) {
                int vaccines = grid2->getFieldByPoint(p).vaccinesToPutMinimal(countryID);
                //this "if" is not needed (see check above)
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

std::vector<Point> AI::mapAddBorderFieldsForDistrict(size_t districtID) {
    //ToDo ezt elég 1x kiszámolni
    std::vector<Point> border;
    for (auto f :grid2->getDistrictByID(districtID).getAssignedFields()) {
        Point p = grid2->getPointByFieldID(f->getFieldID());
        if (p.isBorder())border.push_back(p);

    }
    return border;
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
                    //TODO: refactor from isClear to isInCountry
                    //old: if (!grid2->getDistrictByPoint(p).isClear())
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
