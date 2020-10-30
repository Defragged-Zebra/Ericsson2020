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

class Logic {
    static Grid *grid;

    static int distance(Grid *grid, size_t x1, size_t y1, size_t x2, size_t y2);

    static int calculateCrossInfection(Grid *grid, int centerY, int centerX, uint64_t  factor3);

public:

    Logic() = default;

    void static setGrid(Grid *newGrid) {
        Logic::grid = newGrid;
    }

    void static simulateTO(int gameID, int tickID, int countryID);


    //separate functions, so values can be changed from the visualization directly
    static int calculateSpontaneousHealing(Grid *grid, int centerY, int centerX, int healStartTick);

    static int calculateSpontaneousInfection(Grid *grid, size_t fieldCoordinateX, size_t fieldCoordinateY);
    static void shiftFactor2to4();

    static void shiftXtimesY2to4();
};


#endif //VIRUS_LOGIC_H
