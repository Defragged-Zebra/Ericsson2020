//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_LOGIC_H
#define VIRUS_LOGIC_H

#include <algorithm>
#include <vector>
#include <stdexcept>
#include <cmath>
#include "grid.h"
#include "config.h"
#include <iostream>
#include <stdexcept>
#include "utils.h"

class Logic {
    static Grid *grid;

    static int distance(const Point &p1, const Point &p2);

    static int calculateCrossInfectionLEGACYv1(const Point &center, uint64_t factor3);

    static double calculateCrossInfection(const Point &center, uint64_t factor3);

public:
    Logic() = default;

    void static setGrid(Grid *newGrid) { Logic::grid = newGrid; }

    static Grid *getGrid() { return grid; }

    static void simulateTO(int gameID, size_t tickID, size_t countryID);

    static void simulateVaccination(const std::vector<VaccineData> &back, const std::vector<VaccineData> &put);

    static int calculateSpontaneousHealing(const Point &p, int healStartTick, int vaccinated);

    static int calculateSpontaneousHealingLEGACYv25(const Point &p, int healStartTick, int vaccinated);

    static int calculateSpontaneousHealingLEGACYv1(const Point &p, int healStartTick);

    static int calculateSpontaneousInfection(const Point &p);

    static int calculateSpontaneousInfectionLEGACYv25(const Point &p);

    static int calculateSpontaneousInfectionLEGACYv2(const Point &p);

    static int calculateSpontaneousInfectionLEGACYv1(const Point &p);

    static int calculateVaccination(const Point &p, int &spontaneousHealAmount);

    static void shiftFactor2to4();

    static void calculateBorder(size_t countryID);

    //static void calculateWannabeBorder(size_t countryID);

    static int calculateSpontaneousHealingLEGACYv2(const Point &p, int healStartTick, int vaccinated);
};


#endif //VIRUS_LOGIC_H
