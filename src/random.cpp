//
// Created by lukac on 10/18/2020.
//

#include "random.h"
#include <iostream>

Random::Random(const uint64_t seeds[4]) {
    for (int i = 0; i < 4; ++i) {
        factor[i] = seeds[i];
    }
}

Random &Random::operator=(const Random &r) {
    if (this != &r) {
        for (size_t i = 0; i < 4; ++i) {
            this->factor[i] = r.factor[i];
        }
    }
    return *this;
}

//e.g. index 2 refers to factor2
uint64_t Random::next(int index) {
    factor[index-1] = factor[index-1] * 48271UL % 0x7fffffffUL;
    return factor[index-1];
}

uint64_t Random::getFactor(int index) const {
    return factor[index-1];
}
