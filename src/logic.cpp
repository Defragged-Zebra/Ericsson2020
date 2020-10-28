//
// Created by lukac on 10/18/2020.
//

//#include "mainloop.h"
#include <queue>


#include "logic.h"

Grid *Logic::grid = nullptr;

void Logic::simulateTO(int gameID, int tickID, int countryID) {
    for (int i = 0; (grid->getCurrentTick() < tickID); ++i) {

        //if(currentTick>maxticks){throw std::runtime_error("antiVirus.cpp: too many ticks");}
        int heal=0;
        int inf=0;
        size_t healStartTick = grid->getX() + grid->getY();
        for (int x = 0; x < grid->getX(); ++x) {
            for (int y = 0; y < grid->getY(); ++y) {
                heal = Logic::calculateSpontaneousHealing(grid, x, y, healStartTick);
                inf = Logic::calculateSpontaneousInfection(grid, x, y);
                grid->getFieldByID((*grid)[y][x]).updateVaccination(heal);
                grid->getFieldByID((*grid)[y][x]).updateInfection(inf);
            }
        }
        grid->IncreaseCurrentTick();
    }
}

int Logic::calculateSpontaneousHealing(Grid *grid, int fieldCoordinateX, int fieldCoordinateY,
                                       int healStartTick) {
    size_t currentTick = grid->getCurrentTick();
    if (grid == nullptr) throw std::invalid_argument("grid null pointer");
    Field field = grid->getFieldByID(grid->transformCoordinateToID(fieldCoordinateY, fieldCoordinateX));
    //healStartTick = width + height; -- it should be calculated further up for optimisation
    //currentTick = hanyadik tick van
    //Ha még nem értük el a width + height -edik kört, akkor 0
    //+1 because Tick starts from 0
    if (currentTick + 1 < healStartTick ||
        grid->getFieldByID((*grid)[fieldCoordinateX][fieldCoordinateY]).getCurrentInfectionValue() == 0) {
        return 0;
    } else {
        //Az előző tickek (pályaméret width + height darabszámú) fertőzöttségi mutatóinak minimuma szorozva az ...
        //a = min(lastInfectionValues[lastInfectionValues.len() - lastTicks->lastInfectionValues.len()]);
        unsigned long factor1;
        if (grid->getCurrentTick() == healStartTick) {
            factor1 = grid->random.getFactor(1);
        } else {
            grid->random.next(1);
        }
        std::deque<int>::iterator tmp = std::min_element(field.getLastInfectionValues().begin(),
                                                         field.getLastInfectionValues().end());
        double a = *tmp;
        //első véletlen faktor 10-zel való osztási maradékával (0-9)
        double b = int(factor1 % 10);
        //Az eredmény osztva 20-al, és ennek az alsó egészrésze
        return std::floor((a * b) / 20.0);
    }
}

int Logic::calculateSpontaneousInfection(Grid *grid, size_t fieldCoordinateX, size_t fieldCoordinateY) {
    if (grid == nullptr) throw std::invalid_argument("grid null pointer");

    unsigned long random2, random3, random4;
    /* if (grid->getCurrentTick() == 0) {
        random2 = grid->random.getFactor(2);
        random3 = grid->random.getFactor(3);
        random4 = grid->random.getFactor(4);
    } else { */
        random2 = grid->random.next(2);

        random3 = grid->random.next(3);
        random4 = grid->random.next(4);
    //}

    size_t currentTick = grid->getCurrentTick();
    size_t fieldID = grid->transformCoordinateToID(fieldCoordinateX, fieldCoordinateY);
    size_t districtID = grid->getFieldByID(fieldID).getAssignedDistrictID();
    if (grid->getDistrictByID(districtID).isClear()) {
        return 0;
    } else {
        //curr_tick: ticks elapsed
        //A második véletlen faktor 10-zel való osztási maradéka + 10 darab előző vírusterjedés átlaga az adott cellán.
        //the -1 in the end is there because we later use it as an array index
        int intervalToAverage = std::min(int(random2 % 10) + 10, (int) currentTick) - 1;
        //int b = avg(i =[1..c], infection(curr_tick - i, coord));
        Field &field = grid->getFieldByID(grid->transformCoordinateToID(fieldCoordinateY, fieldCoordinateX));
        //a "rekurzív" függvényhívás miatt szükség van az első elemre, ami a következőképpen néz ki:
        if (currentTick == 0) {
            //infection(0, coord) => tick_info[0, coord].infection_rate > 0 ? 1 : 0
            return field.getCurrentInfectionValue() > 0 ? 1 : 0;
        }
        double b = 0;
        std::deque<int> lastInfectionValues = field.getLastInfectionValues();
        int size = lastInfectionValues.size();
        for (int i = intervalToAverage; i < size; ++i) {
            b += lastInfectionValues[i];
        }
        b = b / size;
        int sum = calculateCrossInfection(grid, fieldCoordinateX, fieldCoordinateY, random3);
        //Ehhez hozzáadva az adott cella és a szomszédjainak az átfertőződési mutatóját.
        double a = b + sum;
        //Az így eddig kiszámolt összeget megszorozzuk a negyedik véletlen faktor 25-tel való osztási maradéka + 50-nel,
        //és az egészet leosztjuk 100-al, majd vesszük a felső egészrészét.
        std::cout << std::ceil(a * (double) (random4 % 25 + 50) / 100.0);
        return std::ceil(a * (double) (random4 % 25 + 50) / 100.0);
    }
}

int Logic::distance(Grid *grid, size_t x1, size_t y1, size_t x2, size_t y2) {
    //A távolság helyben 0, megegyező kerületben 1, egyébként 2
    if (x1 == x2 && y1 == y2) {
        return 0;
    }
    size_t district1 = grid->getFieldByID(grid->transformCoordinateToID(x1, y1)).getAssignedDistrictID();
    size_t district2 = grid->getFieldByID(grid->transformCoordinateToID(x2, y2)).getAssignedDistrictID();
    if (district1 != district2) {
        return 2;
    } else {
        return 1;
    }
}

int Logic::calculateCrossInfection(Grid *grid, int fieldCoordinateX, int fieldCoordinateY, unsigned long random3) {
    Field &field = grid->getFieldByID(grid->transformCoordinateToID(fieldCoordinateY, fieldCoordinateX));
    int sum = 0;
    int coordinates[5][2] = {{fieldCoordinateX,     fieldCoordinateY},
                             {fieldCoordinateX - 1, fieldCoordinateY},
                             {fieldCoordinateX,     fieldCoordinateY + 1},
                             {fieldCoordinateX + 1, fieldCoordinateY},
                             {fieldCoordinateX,     fieldCoordinateY - 1}};
    size_t infectionDequeSize = field.getLastInfectionValues().size() - 1;
    for (int i = 0; i < 5; ++i) {
        int cX = coordinates[i][0];
        int cY = coordinates[i][1];
        //boundary check .. -1 because arrays still start at 0
        if (cX < 0 || cY < 0 || cX > grid->getX() - 1 || cY > grid->getY() - 1) {
            continue;
        }
        int dist = distance(grid, fieldCoordinateX, fieldCoordinateY, cX, cY);
        //Az átfertőződési mutatók kiszámolása előtt a t átfertőződési hajlandóságot generáljuk
        // a harmadik véletlen faktor 7-tel való osztási maradéka + 3 -mal
        int t = int(random3 % 7) + 3;


        //Az átfertőződési mutatókat akkor adjuk össze a hely populációs különbségéből adódó,
        //1-3 érték közé beszorított fertőzési lehetőséggel,
        //ha az előző körös fertőzöttség nagyobb, mint a hajlandóság és a távolság szorzata.
        Field cField = grid->getFieldByID(grid->transformCoordinateToID(cY, cX));
        //for C++ dark magic, we can't get the values immediately out of the deque reference
        std::deque<int> lastInfectionValues = cField.getLastInfectionValues();
        if (lastInfectionValues[infectionDequeSize] > dist * t) {
            //"beszorított átfertőzési mutató"
            //std::clamp added in C++17
            int d = std::clamp(field.getPopulationDensity() - cField.getPopulationDensity(), 0, 2) + 1;
            sum += d;
        } else {
            continue;
        }
    }
    return sum;
}
