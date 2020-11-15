//
// Created by lukac on 10/18/2020.
//

#include <queue>
#include <numeric>


#include "logic.h"

Grid *Logic::grid = nullptr;

void Logic::simulateTO(int gameID, size_t tickID, size_t countryID) {
    for (int i = 0; (grid->getCurrentTick() < tickID); ++i) {

        int heal = 0;
        int vaccination = 0;
        size_t healStartTick = grid->getWidth() + grid->getHeight();
        for (int x = 0; x < grid->getWidth(); ++x) {
            for (int y = 0; y < grid->getHeight(); ++y) {
                const Point &p = Point(y, x);
                vaccination = Logic::calculateVaccination(p, heal);
                heal = Logic::calculateSpontaneousHealing(p, healStartTick, vaccination);
                grid->getFieldByPoint(p).updateVaccination(heal + vaccination);
                grid->getFieldByPoint(p).updateRemainingVaccines(vaccination);
                if (heal + vaccination > 0) {
                    if (grid->getDistrictByPoint(p).updateIsClear()) {
                        //todo: in round3 change this
                        District &district = grid->getDistrictByPoint(p);
                        district.setAssignedCountryID(countryID);
                        grid->getCountryByID(countryID).addAssignedDistrict(&district);
#ifndef PROD
                        int change = grid->calculateChangeInProducedVaccinesByHealingDistrict(countryID,
                                                                                              grid->getDistrictByPoint(
                                                                                                      p));
                        int existing = grid->getCountryByID(countryID).getTotalProductionCapacity();
                        grid->getCountryByID(countryID).setTotalProductionCapacity(existing + change);
                        grid->getCountryByID(countryID).setReserveVaccines(
                                grid->getCountryByID(countryID).getReserveVaccines() +
                                grid->getCountryByID(countryID).getTotalProductionCapacity());
#endif
                    }
                }
            }
        }

        int inf = 0;
        for (int x = 0; x < grid->getWidth(); ++x) {
            for (int y = 0; y < grid->getHeight(); ++y) {
                inf = Logic::calculateSpontaneousInfection(Point(y, x));
                grid->getFieldByPoint(Point(y, x)).updateInfection(inf);
            }
        }

        std::map<size_t, std::set<Point>> nvf;
        grid->getNotVaccinatedFields(countryID, nvf);
        grid->getCountryByID(countryID).thereAreNoVaccinatedFieldsHere(nvf);

        calculateBorder(countryID);

        grid->IncreaseCurrentTick();
    }

}


int Logic::calculateSpontaneousHealingLEGACY(const Point &p, int healStartTick) {
    size_t currentTick = grid->getCurrentTick();
    if (grid == nullptr) throw std::invalid_argument("grid null pointer");
    if (currentTick < healStartTick) {
        return 0;
    } else {
        if (grid->getFieldByPoint(p).getCurrentInfectionRate() == 0) return 0;
        uint64_t factor1 = grid->random.getFactor(1);
        Field field = grid->getFieldByPoint(p);
        size_t beginIndex = field.getLastInfectionRates().size() - grid->getWidth() - grid->getHeight();
        auto tmp = std::min_element(field.getLastInfectionRates().begin() + beginIndex,
                                    field.getLastInfectionRates().end());
        double a = *tmp;
        double b = int(factor1 % 10);
        grid->random.next(1);
        return std::floor((a * b) / 20.0);
    }
}

int Logic::calculateSpontaneousInfection(const Point &p) {

    if (grid == nullptr) throw std::invalid_argument("grid null pointer");
    uint64_t factor2, factor3, factor4;

    size_t currentTick = grid->getCurrentTick();
    size_t districtID = grid->getFieldByPoint(p).getAssignedDistrictID();

    if (grid->getDistrictByID(districtID).isClear()) return 0;

    factor2 = grid->random.getFactor(2);
    factor3 = grid->random.getFactor(3);
    factor4 = grid->random.getFactor(4);
    int intervalToAverage = std::min(int(factor2 % 10) + 10, (int) currentTick + 1);
    Field &field = grid->getFieldByPoint(p);

    double average = 0;
    std::deque<int> &lastInfectionValues = field.getLastInfectionValues();
    int size = lastInfectionValues.size();
    for (int i = 0; i < intervalToAverage; ++i) {
        average += lastInfectionValues[size - i - 1];
    }
    average = average / intervalToAverage;
    //round error
    average = (std::floor(average * 100000.0)) / 100000.0;
    double average2 = calculateCrossInfection(p, factor3);
    double a = average + average2;

    Logic::shiftFactor2to4();
    //round error
    a = (std::floor(a * 100000.0)) / 100000.0;
    return std::ceil(a * (double) (factor4 % 25) / 100.0);
}

int Logic::calculateSpontaneousInfectionLEGACY(const Point &p) {

    if (grid == nullptr) throw std::invalid_argument("grid null pointer");
    uint64_t factor2, factor3, factor4;

    size_t currentTick = grid->getCurrentTick();
    size_t districtID = grid->getFieldByPoint(p).getAssignedDistrictID();
    if (grid->getDistrictByID(districtID).isClear()) {
        return 0;
    } else {
        factor2 = grid->random.getFactor(2);
        factor3 = grid->random.getFactor(3);
        factor4 = grid->random.getFactor(4);
        int intervalToAverage = std::min(int(factor2 % 10) + 10, (int) currentTick + 1);
        Field &field = grid->getFieldByPoint(p);

        double average = 0;
        std::deque<int> &lastInfectionValues = field.getLastInfectionValues();
        int size = lastInfectionValues.size();
        for (int i = 0; i < intervalToAverage; ++i) {
            average += lastInfectionValues[size - i - 1];
        }
        average = average / intervalToAverage;
        //round error
        average = (std::floor(average * 100000.0)) / 100000.0;

        int sum = calculateCrossInfectionLEGACY(p, factor3);

        double a = average + sum;

        Logic::shiftFactor2to4();
        //round error
        a = (std::floor(a * 100000.0)) / 100000.0;
        return std::ceil(a * (double) (factor4 % 25 + 50) / 100.0);
    }
}

void Logic::shiftFactor2to4() {
    grid->random.next(2);
    grid->random.next(3);
    grid->random.next(4);
}

int Logic::distance(const Point &p1, const Point &p2) {
    if (p1 == p2) return 0;
    else if (grid->getDistrictByPoint(p1) != grid->getDistrictByPoint(p2)) return 2;
    else return 1;
}


int Logic::calculateCrossInfectionLEGACY(const Point &center, uint64_t factor3) {
    Field &field = grid->getFieldByPoint(center);
    int sum = 0;
    int t = int(factor3 % 7) + 3;
    size_t centerY = center.getY();
    size_t centerX = center.getX();
    std::vector<Point> coordinates = center.getNeighbours();
    coordinates.push_back(center);
    for (const auto &selected : coordinates) {
        if (!selected.withinBounds()) continue;
        int dist = distance(center, selected);
        Field &selectedField = grid->getFieldByPoint(selected);
        std::deque<int> &lastInfectionRate = selectedField.getLastInfectionRates();
        size_t infectionDequeSize = selectedField.getLastInfectionRates().size() - 1;
        size_t index;
        if (selected.getX() < centerX || selected.getY() < centerY) {
            index = infectionDequeSize - 1;
        } else {
            index = infectionDequeSize;
        }
        if (lastInfectionRate[index] > dist * t) {
            int d = std::clamp(field.getPopulationDensity() - selectedField.getPopulationDensity(), 0, 2) + 1;
            sum += d;
        } else {
            continue;
        }
    }
    return sum;
}


double Logic::calculateCrossInfection(const Point &center, uint64_t factor3) {
    Field &field = grid->getFieldByPoint(center);
    double sum = 0;
    int t = int(factor3 % 7) + 3;
    int elementsToAverage = 5;
    size_t centerY = center.getY();
    size_t centerX = center.getX();
    Point coordinates[5] = {{centerY,     centerX},
                            {centerY,     centerX - 1},
                            {centerY - 1, centerX},
                            {centerY + 1, centerX},
                            {centerY,     centerX + 1}};

    for (const auto &selected : coordinates) {
        if (!selected.withinBounds()) {
            elementsToAverage--;
            continue;
        }
        int dist = distance(center, selected);
        Field &selectedField = grid->getFieldByPoint(selected);
        std::deque<int> &lastInfectionRate = selectedField.getLastInfectionRates();
        size_t infectionDequeSize = selectedField.getLastInfectionRates().size() - 1;
        size_t index;
        if (selected.getX() < centerX || selected.getY() < centerY) {
            index = infectionDequeSize - 1;
        } else {
            index = infectionDequeSize;
        }
        if (lastInfectionRate[index] > dist * t) {
            int d = std::clamp(field.getPopulationDensity() - selectedField.getPopulationDensity(), 0, 2) + 1;
            sum += d;
        } else {
            continue;
        }
    }
    //double precision matters
    return (std::floor(sum / elementsToAverage * 100000.0)) / 100000.0;
}

int Logic::calculateVaccination(const Point &p, int &spontaneousHealAmount) {
    Field &f = grid->getFieldByPoint(p);
    // IR: last infection rate
    int IR = f.getCurrentInfectionRate();
    //P: population density
    int P = f.getPopulationDensity();
    //Ha egy adott területen az IR > 0
    if (IR > 0 && !f.isClear()) {
        //van n db tartalék vakcina az összes országnak együttvéve
        std::map<size_t, int> vaccinesMap = f.getStoredVaccines();
        //reserve vaccines
        int n = 0;
        for (const auto &tmp:vaccinesMap) {
            n += tmp.second;
        }
        //X: vaccination rate
        //akkor az adott terület infectionRate-je csökkenni, a healthRate-je nőni fog X = min(n * P, IR) -nal
        return std::min(n * P, IR);
    }
    return 0;
}

int Logic::calculateSpontaneousHealing(const Point &p, int healStartTick, int vaccinated) {
    size_t currentTick = grid->getCurrentTick();
    if (grid == nullptr) throw std::invalid_argument("grid null pointer");
    if (currentTick < healStartTick) {
        return 0;
    } else {
        if (grid->getFieldByPoint(p).getCurrentInfectionRate() == 0) return 0;
        uint64_t factor1 = grid->random.getFactor(1);
        Field field = grid->getFieldByPoint(p);
        size_t beginIndex = field.getLastInfectionRates().size() - grid->getWidth() - grid->getHeight();
        auto tmp = std::min_element(field.getLastInfectionRates().begin() + beginIndex,
                                    field.getLastInfectionRates().end());
        double a = *tmp;
        double b = int(factor1 % 10);
        grid->random.next(1);
        int h = std::floor((a * b) / 20.0);
        //A h = healing() vissztérési értéke ettől fogva csak floor(h * (IR - X) / IR) -nyit gyógyít.
        return std::floor(h * (grid->getFieldByPoint(p).getCurrentInfectionRate() - vaccinated) /
                          grid->getFieldByPoint(p).getCurrentInfectionRate());
    }
}

void Logic::simulateVaccination(const std::vector<VaccineData> &back, const std::vector<VaccineData> &put) {
    for (const auto &b:back) {
        grid->getFieldByPoint(b.getPoint()).callBackVaccines(b.getVaccines(), b.getCounrtyID());
    }
    for (const auto &p:put) {
        grid->getFieldByPoint(p.getPoint()).pushVaccines(p.getVaccines(), p.getCounrtyID());
    }
}

void Logic::calculateBorder(size_t countryID) {
    std::set<Point> border;
    for (auto d:grid->getCountryByID(countryID).getAssignedDistricts()) {
        for (auto f:d->getAssignedFields()) {
            auto center = Point(f->getFieldID());
            for (const auto &p:center.getNeighbours()) {
                if (grid->getDistrictByPoint(p) != grid->getDistrictByPoint(center)) {
                    if (!grid->getDistrictByPoint(p).isClear()) {
                        border.insert(center);
                        break;
                    }
                }
            }
        }
    }
    grid->getCountryByID(countryID).setBorder(border);
}
