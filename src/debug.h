//
// Created by woranhun on 2020. 10. 30..
//

#ifndef VIRUS_DEBUG_H
#define VIRUS_DEBUG_H
#include <iostream>
#include "utils.h"
#include "config.h"
void debug() {
    u_long factors[4] = {1569741360, 1785505948, 516548029, 1302116447};
    Utils::Random rd = Utils::Random(factors);
    size_t i = 0;
    int index = 4;
    bool overrun[4] = {false, false, false, false};
    do {
        if (rd.getFactor(index) == 1022357306) { std::cout << "i at 1022357306 = " << i << std::endl; }
        if (rd.getFactor(index) == 6014329) { std::cout << "i at 6014329 = " << i << std::endl; }
        if (rd.getFactor(index) == 383700820) { std::cout << "i at 383700820 = " << i << std::endl; }
        if (rd.getFactor(index) == 1880906878) { std::cout << "i at 1880906878 = " << i << std::endl; }
        ++i;
    } while (rd.next(index) != 49058578); //6014329);
    std::cout << "i= " << i << std::endl;
    do {
        std::cout << "i= " << i << " \tfactor2: " << rd.getFactor(2) << "    \tfactor3: " << rd.getFactor(3)
                  << "   \tfactor4: " << rd.getFactor(4) << std::endl;
        rd.next(2);
        rd.next(3);
        rd.next(4);
        ++i;
    } while (rd.getFactor(2) != 6014329);
    std::cout << "i= " << i << " \tfactor2: " << rd.getFactor(2) << "    \tfactor3: " << rd.getFactor(3)
              << " \tfactor4: " << rd.getFactor(4) << std::endl;
    //std::cout << "i= " << i << " factor: " << rd.getFactor(index) << std::endl;
    /*do  {
        ++i;
    } while (rd.next(index) != 413011888); //6014329);
    std::cout << "i= " << i << std::endl;*/
}

void debug2() {
    u_long factors[4] = {1569741360, 1785505948, 516548029, 1302116447};
    auto rd = Utils::Random(factors);
    int i = 1;

    while (i < 240) {
        if ((rd.next(3) % 7)+3 <4){ std::cout << "i= " << i << std::endl; }
        ++i;
    }

}
void debug3(){
    double a = 8.333333333333339;
    a = ((int)(a*100000000.0))/100000000.0;
    std::cout<< a;
}
#endif //VIRUS_DEBUG_H
