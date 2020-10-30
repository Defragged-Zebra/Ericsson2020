//
// Created by lukac on 10/18/2020.
//

//#include "mainloop.h"
#include <queue>


#include "logic.h"

Grid *Logic::grid = nullptr;

void Logic::simulateTO(int gameID, int tickID, int countryID) {
    for (int i = 0; (grid->getCurrentTick() < tickID); ++i) {
        grid->IncreaseCurrentTick();
        if(grid->getCurrentTick()==1){
            //Logic::shiftXtimesY2to4();
            //Logic::shiftXtimesY2to4();
            /*for (int j = 0; j < 21; ++j) {
                Logic::shiftFactor2to4();
            }*/

        }
        //if(currentTick>maxticks){throw std::runtime_error("antiVirus.cpp: too many ticks");}
        int heal = 0;
        int inf = 0;
        size_t healStartTick = grid->getX() + grid->getY();
        for (int x = 0; x < grid->getX(); ++x) {
            for (int y = 0; y < grid->getY(); ++y) {
                heal = Logic::calculateSpontaneousHealing(grid, y, x, healStartTick);
                grid->getFieldByID((*grid)[y][x]).updateVaccination(heal);
            }
        }
        for (int x = 0; x < grid->getX(); ++x) {
            for (int y = 0; y < grid->getY(); ++y) {
                inf = Logic::calculateSpontaneousInfection(grid, y, x);
                inf = std::min(inf, 100 - grid->getFieldByID((*grid)[y][x]).getCurrentInfectionRate() -
                                    grid->getFieldByID((*grid)[y][x]).getVaccinationRate());
                grid->getFieldByID((*grid)[y][x]).updateInfection(inf);
            }
        }
    }
}


int Logic::calculateSpontaneousHealing(Grid *grid, int centerY, int centerX,
                                       int healStartTick) {
    size_t currentTick = grid->getCurrentTick();
    if (grid == nullptr) throw std::invalid_argument("grid null pointer");
    //TODO: might be bad implementation
    //currentTick==11 -> healing start!
    if (currentTick <= healStartTick) {
        return 0;
    } else {
        if(grid->getFieldByID((*grid)[centerY][centerX]).getCurrentInfectionRate() == 0) return 0;
        uint64_t factor1 = grid->random.getFactor(1);
        Field field = grid->getFieldByID(grid->transformCoordinateToID(centerY, centerX));
        //healStartTick = width + height; -- it should be calculated further up for optimisation
        //currentTick = hanyadik tick van
        //Ha még nem értük el a width + height -edik kört, akkor 0
        //Az előző tickek (pályaméret width + height darabszámú) fertőzöttségi mutatóinak minimuma szorozva az ...
        //a = min(lastInfectionValues[lastInfectionValues.len() - lastTicks->lastInfectionValues.len()]);
        size_t beginIndex =field.getLastInfectionRates().size()-grid->getX()-grid->getY();
        std::deque<int>::iterator tmp = std::min_element(field.getLastInfectionRates().begin()+beginIndex,
                                                         field.getLastInfectionRates().end());
        double a = *tmp;
        //első véletlen faktor 10-zel való osztási maradékával (0-9)
        double b = int(factor1 % 10);
        //Az eredmény osztva 20-al, és ennek az alsó egészrésze
        grid->random.next(1);
        return std::floor((a * b) / 20.0);
    }
}

int Logic::calculateSpontaneousInfection(Grid *grid, size_t fieldCoordinateY, size_t fieldCoordinateX) {

    if (grid == nullptr) throw std::invalid_argument("grid null pointer");
    uint64_t factor2, factor3, factor4;

    size_t currentTick = grid->getCurrentTick();
    size_t fieldID = grid->transformCoordinateToID(fieldCoordinateY, fieldCoordinateX);
    size_t districtID = grid->getFieldByID(fieldID).getAssignedDistrictID();
    if (grid->getDistrictByID(districtID).isClear()) {
        return 0;
    } else {
        factor2 = grid->random.getFactor(2);
        factor3 = grid->random.getFactor(3);
        factor4 = grid->random.getFactor(4);
        //curr_tick: ticks elapsed
        //A második véletlen faktor 10-zel való osztási maradéka + 10 darab előző vírusterjedés átlaga az adott cellán.
        //the -1 in the end is there because we later use it as an array index
        int intervalToAverage = std::min(int(factor2 % 10) + 10, (int) currentTick);
        //int average = avg(i =[1..c], infection(curr_tick - i, coord));
        Field &field = grid->getFieldByID(grid->transformCoordinateToID(fieldCoordinateY, fieldCoordinateX));
        //a "rekurzív" függvényhívás miatt szükség van az első elemre, ami a következőképpen néz ki:

        /*if (currentTick == 1) {
            //infection(0, coord) => tick_info[0, coord].infection_rate > 0 ? 1 : 0
            return field.getCurrentInfectionRate() > 0 ? 1 : 0;
        }*/

        double average = 0;
        std::deque<int> lastInfectionValues = field.getLastInfectionValues();
        int size = lastInfectionValues.size();
        for (int i = 0; i < intervalToAverage; ++i) {
            average += lastInfectionValues[size - i - 1];
        }
        average = average / intervalToAverage;
        int sum = calculateCrossInfection(grid, fieldCoordinateY, fieldCoordinateX, factor3);
        //Ehhez hozzáadva az adott cella és a szomszédjainak az átfertőződési mutatóját.
        double a = average + sum;
        //Az így eddig kiszámolt összeget megszorozzuk a negyedik véletlen faktor 25-tel való osztási maradéka + 50-nel,
        //és az egészet leosztjuk 100-al, majd vesszük a felső egészrészét.
        Logic::shiftFactor2to4();
        return std::ceil(a * (double) (factor4 % 25 + 50) / 100.0);
    }
}
void Logic::shiftXtimesY2to4(){
    for (size_t i = 0; i < grid->getX()*grid->getY(); ++i) {
        shiftFactor2to4();
    }
}
void Logic::shiftFactor2to4() {
    grid->random.next(2);
    grid->random.next(3);
    grid->random.next(4);
}

int Logic::distance(Grid *grid, size_t x1, size_t y1, size_t x2, size_t y2) {
    //A távolság helyben 0, megegyező kerületben 1, egyébként 2
    if (x1 == x2 && y1 == y2) {
        return 0;
    }
    size_t district1 = grid->getFieldByID(grid->transformCoordinateToID(y1, x1)).getAssignedDistrictID();
    size_t district2 = grid->getFieldByID(grid->transformCoordinateToID(y2, x2)).getAssignedDistrictID();
    if (district1 != district2) {
        return 2;
    } else {
        return 1;
    }
}

int Logic::calculateCrossInfection(Grid *grid, int centerY, int centerX, uint64_t factor3) {
    Field &field = grid->getFieldByID(grid->transformCoordinateToID(centerY, centerX));
    int sum = 0;
    //Az átfertőződési mutatók kiszámolása előtt a t átfertőződési hajlandóságot generáljuk
    // a harmadik véletlen faktor 7-tel való osztási maradéka + 3 -mal
    int t = int(factor3 % 7) + 3;
    int coordinates[5][2] = {{centerY,centerX},
                             {centerY, centerX-1},
                             {centerY-1, centerX},
                             {centerY+1,centerX},
                             {centerY, centerX+1}};
    for (int i = 0; i < 5; ++i) {
        int selectedY = coordinates[i][0];
        int selectedX = coordinates[i][1];
        //boundary check .. -1 because arrays still start at 0
        if (selectedX < 0 || selectedY < 0 || selectedX > grid->getX() - 1 || selectedY > grid->getY() - 1) {
            continue;
        }
        int dist = distance(grid, centerX, centerY, selectedX, selectedY);
        //Az átfertőződési mutatókat akkor adjuk össze a hely populációs különbségéből adódó,
        //1-3 érték közé beszorított fertőzési lehetőséggel,
        //ha az előző körös fertőzöttség nagyobb, mint a hajlandóság és a távolság szorzata.
        Field selectedField = grid->getFieldByID(grid->transformCoordinateToID(selectedY, selectedX));
        //for C++ dark magic, we can't get the values immediately out of the deque reference
        std::deque<int> lastInfectionRate = selectedField.getLastInfectionRates();
        //last infection value, or the previous one
        size_t infectionDequeSize = selectedField.getLastInfectionRates().size() - 1;
        size_t index;
        //std::cerr<<"cros_cell_check between: "<<centerY<<" "<<centerX<<"; "<<selectedY<<" "<<selectedX<<std::endl;
        if (selectedX < centerX || selectedY < centerY){
            index = infectionDequeSize - 1;
        } else {
            index = infectionDequeSize;
        }
        if (lastInfectionRate[index] > dist * t) {
            //"beszorított átfertőzési mutató"
            int d = std::clamp(field.getPopulationDensity() - selectedField.getPopulationDensity(), 0, 2) + 1;
            sum += d;
        } else {
            continue;
        }
    }
    return sum;
}
