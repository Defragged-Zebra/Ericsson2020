//
// Created by woranhun on 2020. 11. 01..
//

#include "utils.h"

Utils::Random::Random(const uint64_t seeds[4]) {
    for (int i = 0; i < 4; ++i) {
        factor[i] = seeds[i];
    }
}

Utils::Random &Utils::Random::operator=(const Random &r) {
    if (this != &r) {
        for (size_t i = 0; i < 4; ++i) {
            this->factor[i] = r.factor[i];
        }
    }
    return *this;
}

//e.g. index 2 refers to factor2
uint64_t Utils::Random::next(int index) {
    factor[index-1] = factor[index-1] * 48271UL % 0x7fffffffUL;
    return factor[index-1];
}

uint64_t Utils::Random::getFactor(int index) const {
    return factor[index-1];
}