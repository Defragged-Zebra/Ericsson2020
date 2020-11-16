//
// Created by woranhun on 2020. 11. 02..
//
#include "config.h"
#ifndef PROD
#include "visualization.h"

void Visualization::displayCurrentRound(int gameID, int tickID, int countryID) {
    os << "RES " << gameID << " " << tickID << " " << countryID << std::endl;
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            os << grid->getFieldByID((*grid)[y][x]).getCurrentInfectionRate() << " ";
        }
        os << std::endl;
    }
    os << "." << std::endl;

    ablak.update();
    // show windows comes here
}
#endif
