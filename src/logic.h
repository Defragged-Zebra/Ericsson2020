//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_LOGIC_H
#define VIRUS_LOGIC_H

#include <algorithm>
#include <vector>
#include <stdexcept>
//in the end I included this bc they surely not asking to write such low lvl funcs like floor(double n) as those require bitshifting stuff;
#include <cmath>
#include "grid.h"

//for debug purposes
#include <iostream>
#include <stdexcept>
#include "utils.h"

class Logic {
    static Grid *grid;

    static int distance(const Point &p1, const Point &p2);

    static int calculateCrossInfection(const Point &center, uint64_t factor3);

public:

    Logic() = default;

    void static setGrid(Grid *newGrid) {
        Logic::grid = newGrid;
    }

    void static simulateTO(int gameID, int tickID, int countryID);

    static int calculateSpontaneousHealing(const Point &p, int healStartTick);

    static int calculateSpontaneousInfection(const Point &p);

    static void shiftFactor2to4();

    static std::vector<VaccineData> &calculateBackVaccines(std::vector<VaccineData> &back, size_t tickID);

    static std::vector<VaccineData> &calculatePutVaccines(std::vector<VaccineData> &put, size_t tickID);

};


#endif //VIRUS_LOGIC_H
