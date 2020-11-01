//
// Created by lukac on 10/18/2020.
//

#include <queue>


#include "logic.h"

Grid *Logic::grid = nullptr;

void Logic::simulateTO(int gameID, int tickID, int countryID) {
    for (int i = 0; (grid->getCurrentTick() < tickID); ++i) {

        int heal = 0;
        size_t healStartTick = grid->getWidth() + grid->getHeight();
        for (int x = 0; x < grid->getWidth(); ++x) {
            for (int y = 0; y < grid->getHeight(); ++y) {
                heal = Logic::calculateSpontaneousHealing(Point(y,x),healStartTick);
                grid->getFieldByPoint(Point(y,x)).updateVaccination(heal);
            }
        }

        int inf = 0;
        for (int x = 0; x < grid->getWidth(); ++x) {
            for (int y = 0; y < grid->getHeight(); ++y) {
                inf = Logic::calculateSpontaneousInfection(Point(y, x));
                grid->getFieldByPoint(Point(y, x)).updateInfection(inf);
            }
        }
        grid->IncreaseCurrentTick();
    }
}


int Logic::calculateSpontaneousHealing(const Point& p, int healStartTick) {
    size_t currentTick = grid->getCurrentTick();
    if (grid == nullptr) throw std::invalid_argument("grid null pointer");
    if (currentTick < healStartTick) {
        return 0;
    } else {
        if(grid->getFieldByPoint(p).getCurrentInfectionRate() == 0) return 0;
        uint64_t factor1 = grid->random.getFactor(1);
        Field field = grid->getFieldByPoint(p);
        size_t beginIndex = field.getLastInfectionRates().size() - grid->getWidth() - grid->getHeight();
        auto tmp = std::min_element(field.getLastInfectionRates().begin()+beginIndex,
                                                         field.getLastInfectionRates().end());
        double a = *tmp;
        double b = int(factor1 % 10);
        grid->random.next(1);
        return std::floor((a * b) / 20.0);
    }
}

int Logic::calculateSpontaneousInfection(const Point& p) {

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
        int intervalToAverage = std::min(int(factor2 % 10) + 10, (int) currentTick+1);
        Field &field = grid->getFieldByPoint(p);

        double average = 0;
        std::deque<int>& lastInfectionValues = field.getLastInfectionValues();
        int size = lastInfectionValues.size();
        for (int i = 0; i < intervalToAverage; ++i) {
            average += lastInfectionValues[size - i - 1];
        }
        average = average / intervalToAverage;
        //round error
        average = (std::floor(average*100000.0))/100000.0;
        int sum = calculateCrossInfection(p, factor3);
        double a = average + sum;

        Logic::shiftFactor2to4();
        //round error
        a = (std::floor(a*100000.0))/100000.0;
        return std::ceil(a * (double) (factor4 % 25 + 50) / 100.0);
    }
}
void Logic::shiftFactor2to4() {
    grid->random.next(2);
    grid->random.next(3);
    grid->random.next(4);
}

int Logic::distance(const Point& p1, const Point& p2) {
    if (p1==p2) return 0;
    else if (grid->getDistrictByPoint(p1) != grid->getDistrictByPoint(p2)) return 2;
    else return 1;
}

int Logic::calculateCrossInfection(const Point& center, uint64_t factor3) {
    Field &field = grid->getFieldByPoint(center);
    int sum = 0;
    int t = int(factor3 % 7) + 3;
    size_t centerY= center.getY();
    size_t centerX= center.getX();
    Point coordinates[5]={{centerY,centerX},
                         {centerY, centerX-1},
                         {centerY-1, centerX},
                         {centerY+1,centerX},
                         {centerY, centerX+1}};
    for (const auto& selected : coordinates) {
        if ( selected.getX() < 0 || selected.getY() < 0 || selected.getX() > grid->getWidth() - 1 || selected.getY() > grid->getHeight() - 1) {
            continue;
        }
        int dist = distance(center, selected);
        Field& selectedField = grid->getFieldByPoint(selected);
        std::deque<int>& lastInfectionRate = selectedField.getLastInfectionRates();
        size_t infectionDequeSize = selectedField.getLastInfectionRates().size() - 1;
        size_t index;
        if (selected.getX() < centerX || selected.getY() < centerY){
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
