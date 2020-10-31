//
// Created by lukac on 10/18/2020.
//

#ifndef VIRUS_RANDOM_H
#define VIRUS_RANDOM_H

#include <stdexcept>

class Random {
private:
    uint64_t factor[4]{};
public:
    Random() = default;

    explicit Random(const uint64_t seeds[4]);

    Random(const Random &r) {
        *this = r;
    }

    Random &operator=(const Random &r);

    //e.g. index 2 refers to factor2
    uint64_t next(int index);

    [[nodiscard]] uint64_t getFactor(int index) const;
};


#endif //VIRUS_RANDOM_H
