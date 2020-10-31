//
// Created by woranhun on 2020. 10. 17..
//

#include "graphics.h"
#include <iostream>
//for nanosleep
#include <time.h>


void Graphics::start() {
    terminalGraphicsStart();
}

void Graphics::terminalGraphicsStart() {
    size_t x = grid->getWidth();
    size_t y = grid->getHeight();
    std::cout << "initial setup:" << std::endl;
    std::cout << "x=" << x << ", y=" << y << std::endl;
    terminalGraphicsRun(0);
}

void Graphics::terminalGraphicsRun(size_t tickID) {
    std::cout<<std::endl;
    std::system("clear");
    std::cout << "State after tick: " << tickID << std::endl;
    size_t x = grid->getWidth();
    size_t y = grid->getHeight();
    std::cout << "\033[0m+";
    for (int i = 0; i < y; ++i) {
        std::cout << "-";
    }
    std::cout << "+" << std::endl;
    int color; //= 30; //black, codes here: https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
    int infectionValue, healValue;
    int activeTransmitters;
    std::string outputText;
    for (int i = 0; i < x; ++i) {
        std::cout << "\033[0m| ";
        for (int j = 0; j < y; ++j) {
            infectionValue = grid->getFieldByID((*grid)[i][j]).getCurrentInfectionRate();
            healValue = grid->getFieldByID((*grid)[i][j]).getVaccinationRate();
            activeTransmitters=(infectionValue-healValue);
            if (activeTransmitters == 0) {
                color = 30;
            } else if (activeTransmitters < 10) {
                color = 34;
            } else if (activeTransmitters < 20) {
                color = 36;
            } else if (activeTransmitters < 40) {
                color = 32;
            } else if (activeTransmitters < 80) {
                color = 33;
            } else if (activeTransmitters <= 100) {
                color = 31;
            } else {
                color = 35;
            }
            //debug mode for healing
            /*
            if (healValue < 10) {
                std::cout << "\033[" << color << "m"<< healValue<<"   ";
            } else if(healValue<100) {
                std::cout << "\033[" << color << "m"<< healValue<<"  ";
            } else{
                std::cout << "\033[" << color << "m"<< healValue<<" ";
            }*/
            std::cout << "\033[" << color << "m* ";
        }
        std::cout << "\033[0m|" << std::endl;
    }
    std::cout << "\033[0m";
    std::cout << "+";
    for (int i = 0; i < y; ++i) {
        std::cout << "-";
    }
    std::cout << "+" << std::endl;
    /*
    struct timespec tim, tim2;
    tim.tv_sec  = 0;
    tim.tv_nsec = 500000000L;
    nanosleep(&tim , &tim);
     */
}

void Graphics::update(size_t tickID) {
    terminalGraphicsRun(tickID);
}
