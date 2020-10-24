//
// Created by woranhun on 2020. 10. 17..
//

#include "graphics.h"
#include <iostream>

void Graphics::start() {
    terminalGraphics();
}

void Graphics::terminalGraphics() {
    size_t x = grid->getX();
    size_t y = grid->getY();
    std::cout << "x=" << x << ", y=" << y << std::endl;
    int color = 30; //black, codes here: https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
    std::cout << "\033[" << color << "mtest text\n\033[0m\n";
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            int infectionValue = grid->getFieldByID((*grid)[i][j]).getCurrentInfectionValue();
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
            std::cout << "\033[" << color << "mtest_text";
        }
        std::cout << std::endl;
    }
    std::cout << "\033[0m\n";
}
