//
// Created by woranhun on 2020. 10. 17..
//

#include "protocol.h"
#include "logic.h"
#include <cmath>

void Protocol::start() {
    std::string line;
    while (std::getline(is, line)) {
        if (line != ".") {
            Protocol::request(line);
        }
    }
}

void Protocol::request(std::string &line) {
    std::string tmp;
    std::stringstream ss;
    ss << line;
    int gameID, tickID, countryID;
    ss >> tmp >> gameID >> tickID >> countryID;
    //ers<<"Factors before simulation: ";
    //for (int i = 1; i <= 4; ++i) {
    //    ers<<grid->random.getFactor(i)<<" ";
    // }
    //ers<<std::endl;
    //if (grid->getCurrentTick()==0){Logic::shiftFactor2to4(); }
    Logic::simulateTO(gameID, tickID, countryID);

    Protocol::currentResult(gameID, tickID, countryID);
    //ers<<std::endl<<"Factors after simulation: ";
    //for (int i = 1; i <= 4; ++i) {
    //ers<<grid->random.getFactor(i)<<" ";
    //}
    //ers<<std::endl;
    //ers<<"-----------------------"<<std::endl;
}

void Protocol::currentResult(int gameID, int tickID, int countryID) {
    os << "RES " << gameID << " " << tickID << " " << countryID << std::endl;
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            os << grid->getFieldByID((*grid)[y][x]).getCurrentInfectionRate() << " ";
            //if(grid->getCurrentTick()==25&&y==4&&x==6)std::cerr<<"Curr rate: "<<grid->getFieldByID((*grid)[y][x]).getCurrentInfectionRate()<<std::endl;
        }
        os << std::endl;
    }
    os << "." << std::endl;
}

void Protocol::sendDebugMsg(const std::string &msg) {
    ers << msg << std::endl;
}
