//
// Created by woranhun on 2020. 10. 17..
//

#include "protocol.h"
#include "logic.h"
#include <cmath>

void Protocol::displayResult(int gameID, int tickID, int countryID) {
    ers << "[DEBUG]"<<std::endl;
    ers <<"RES " << gameID << " " << tickID << " " << countryID << std::endl;
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            ers << grid->getFieldByID((*grid)[y][x]).getCurrentInfectionRate() << " ";
            //if(grid->getCurrentTick()==25&&y==4&&x==6)std::cerr<<"Curr rate: "<<grid->getFieldByID((*grid)[y][x]).getCurrentInfectionRate()<<std::endl;
        }
        ers << std::endl;
    }
    ers << "[DEBUG VEGE]" <<std::endl;
}
