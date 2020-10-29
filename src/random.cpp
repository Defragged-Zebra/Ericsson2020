//
// Created by lukac on 10/18/2020.
//

#include "random.h"
#include <iostream>

Random::Random (const uint64_t  seeds[4]) {
    for (int i = 0; i < 4; ++i) {
        factor[i] = seeds[i];
    }
}

//e.g. index 2 refers to factor2
uint64_t  Random::next(int index) {
    index --;
    factor[index] = factor[index] * 48271UL % 0x7fffffffUL;
    if (index==2){    std::cout<<"counter: "<<++counter<<std::endl;}
    return factor[index];
}

uint64_t  Random::getFactor (int index) const{
    index --;
    return factor[index];
}
