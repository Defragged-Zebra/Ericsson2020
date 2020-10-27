//
// Created by lukac on 10/18/2020.
//

#ifndef VIRUS_RANDOM_H
#define VIRUS_RANDOM_H

#include <stdexcept>

class Random {
private:
    unsigned long factor[4]{};
public:
    Random(unsigned long seeds[4]);

    Random(const Random &r) {
        *this = r;
    }

    Random() = default;

    Random &operator=(const Random &r) {
        if (this != &r) {
            for (size_t i = 0; i < 4; ++i) {
                this->factor[i] = r.factor[i];
            }
        }
        return *this;
    }
    unsigned long getFactor(int index);
    unsigned long next(int index);
};


#endif //VIRUS_RANDOM_H
