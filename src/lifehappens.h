//
// Created by lukac on 10/18/2020.
//

#ifndef VIRUS_LIFEHAPPENS_H
#define VIRUS_LIFEHAPPENS_H

#include <vector>
#include "grid.h"
#include "random.h"
#include <algorithm>
#include <vector>
#include <stdexcept>
//in the end I included this bc they surely not asking to write such low lvl funcs like floor(double n) as those require bitshifting stuff;
#include <cmath>

//for debug purposes
#include <iostream>
#include <stdexcept>


class LifeHappens {
private:
    /*
    static inline int floor(double n) {
        return n - (n % 1);

    }

    static inline int ceil(double n) {
        return floor(n) + 1;
    }

    static inline int min(int n, int m) {
        return n < m ? n : m;
    }
     */

    static int distance(Grid *grid, size_t x1, size_t y1, size_t x2, size_t y2);

    static int calculateCrossInfection(Grid *grid, int fieldCoordinateX, int fieldCoordinateY, unsigned long random3);

public:
    //separate functions, so values can be changed from the visualization directly
    int calculateSpontaneousHealing(Grid *grid, int fieldCoordinateX, int fieldCoordinateY, int currentTick,
                                    int healStartTick, unsigned long random1);

    int calculateSpontaneousInfection(Grid *grid, int fieldCoordinateX, int fieldCoordinateY, int currentTick,
                                      unsigned long random2, unsigned long random3, unsigned long random4);


};


#endif //VIRUS_LIFEHAPPENS_H
