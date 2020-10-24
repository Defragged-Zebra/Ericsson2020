//
// Created by woranhun on 2020. 10. 17..
//

#include "graphics.h"
#include <iostream>

void Graphics::start() {
    terminalGraphicsStart();
}

void Graphics::terminalGraphicsStart() {
    size_t x = grid->getX();
    size_t y = grid->getY();
    std::cout << "initial setup:" << std::endl;
    std::cout << "x=" << x << ", y=" << y << std::endl;
    terminalGraphicsRun(0);
}

void Graphics::terminalGraphicsRun(size_t tickID) {
    std::cout << "State after tick: " << tickID << std::endl;
    size_t x = grid->getX();
    size_t y = grid->getY();
    std::cout << "\033[0m+";
    for (int i = 0; i < y; ++i) {
        std::cout << "-";
    }
    std::cout << "+" << std::endl;
    int color; //= 30; //black, codes here: https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
    int infectionValue;
    for (int i = 0; i < x; ++i) {
        std::cout << "\033[0m|";
        for (int j = 0; j < y; ++j) {
            infectionValue = grid->getFieldByID((*grid)[i][j]).getCurrentInfectionValue();
            if (infectionValue == 0) {
                color = 30;
            } else if (infectionValue < 10) {
                color = 32;
            } else if (infectionValue < 50) {
                color = 33;
            } else if (infectionValue <= 100) {
                color = 31;
            } else {
                color = 35;
            }
            std::cout << "\033[" << color << "m*";
        }
        std::cout << "\033[0m|" << std::endl;
    }
    std::cout << "\033[0m";
    std::cout << "+";
    for (int i = 0; i < y; ++i) {
        std::cout << "-";
    }
    std::cout << "+" << std::endl;

}

void Graphics::update(size_t tickID) {
    terminalGraphicsRun(tickID);
}
