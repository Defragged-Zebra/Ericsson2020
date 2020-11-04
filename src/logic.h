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

    static int calculateCrossInfectionLEGACY(const Point &center, uint64_t factor3);

    static double calculateCrossInfection(const Point &center, uint64_t factor3);

public:

    Logic() = default;

    void static setGrid(Grid *newGrid) {
        Logic::grid = newGrid;
    }

    void static simulateTO(int gameID, int tickID, int countryID);

    static int calculateSpontaneousHealing(const Point &p, int healStartTick, int vaccinated);
    static int calculateSpontaneousHealingLEGACY(const Point &p, int healStartTick);

    static int calculateSpontaneousInfection(const Point &p);

    static int calculateSpontaneousInfectionLEGACY(const Point &p);

    static int calculateVaccination(const Point &p, int &spontaneousHealAmount);

    static void shiftFactor2to4();

};


#endif //VIRUS_LOGIC_H
