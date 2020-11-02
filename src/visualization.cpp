//
// Created by woranhun on 2020. 11. 02..
//

#include "visualization.h"

void Visualization::displayResult(int gameID, int tickID, int countryID) {
    os << "RES " << gameID << " " << tickID << " " << countryID << std::endl;
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            os << grid->getFieldByID((*grid)[y][x]).getCurrentInfectionRate() << " ";
        }
        os << std::endl;
    }
    os << "." << std::endl;
    // show windows comes here
}
