//
// Created by lukac on 10/18/2020.
//

#ifndef VIRUS_MAINLOOP_H
#define VIRUS_MAINLOOP_H

#include <vector>
#include "grid.h"
//#include "random.h"
#include <algorithm>
#include <vector>
#include <stdexcept>
//in the end I included this bc they surely not asking to write such low lvl funcs like floor(double n) as those require bitshifting stuff;
#include <cmath>

//for debug purposes
#include <iostream>
#include <stdexcept>


class MainLoop {
private:
    static int distance(Grid *grid, size_t x1, size_t y1, size_t x2, size_t y2);

    static int calculateCrossInfection(Grid *grid, int fieldCoordinateX, int fieldCoordinateY, unsigned long random3);

public:
    //separate functions, so values can be changed from the visualization directly
    int calculateSpontaneousHealing(Grid *grid, int fieldCoordinateX, int fieldCoordinateY, int currentTick,
                                    int healStartTick, unsigned long random1);

    int calculateSpontaneousInfection(Grid *grid, int fieldCoordinateX, int fieldCoordinateY, int currentTick,
                                      unsigned long random2, unsigned long random3, unsigned long random4);


};


#endif //VIRUS_MAINLOOP_H
