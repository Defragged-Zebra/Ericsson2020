//
// Created by lukac on 10/18/2020.
//

#ifndef VIRUS_RANDOM_H
#define VIRUS_RANDOM_H

#include <stdexcept>

class Random {
private:
    uint64_t factor[4]{};
    size_t counter=0;
public:
    Random(const uint64_t seeds[4]);

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

    uint64_t getFactor(int index) const;

    uint64_t next(int index);
};


#endif //VIRUS_RANDOM_H
