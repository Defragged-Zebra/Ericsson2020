//
// Created by lukac on 10/18/2020.
//

#ifndef VIRUS_RANDOM_H
#define VIRUS_RANDOM_H

#include <stdexcept>

class Random {
private:
    unsigned long factor[4];
public:
    Random(unsigned long seeds[4]);
    Random(const Random&){
        throw std::runtime_error("Random (that class) copy ctr");
    }
    Random& operator=(const Random&){
        throw std::runtime_error("Random (that class) operator=");
    }
    unsigned long next(int index);
};


#endif //VIRUS_RANDOM_H
