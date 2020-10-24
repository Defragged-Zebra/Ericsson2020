//
// Created by lukac on 10/18/2020.
//

#include "random.h"

Random::Random(unsigned long seeds[4]) {
    for (int i = 0; i < 4; ++i) {
        factor[i] = seeds[i];
    }
}

//e.g. index 2 refers to factor2
unsigned long Random::next(int index) {
    index-=1;
    factor[index] = factor[index] * 48271 % 0x7fffffff;
    return factor[index];
}