//
// Created by woranhun on 2020. 11. 02..
//

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

    ablak.update(std::pair(Point(-1,-1),0));
    // show windows comes here
}

void Visualization::displayCurrentRound(int gameID, int tickID, int countryID, std::pair<Point, int> pair) {
    ablak.update(pair);
    // show windows comes here
}
