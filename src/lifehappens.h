//
// Created by lukac on 10/18/2020.
//

#ifndef VIRUS_LIFEHAPPENS_H
#define VIRUS_LIFEHAPPENS_H

#include <vector>
#include "field.cpp"
//for debug purposes
#include <iostream>
#include <stdexcept>

class LifeHappens {
public:
    int applySpontaneousHealing(std::vector<Field>);
    int applySpontaneousInfection();
};


#endif //VIRUS_LIFEHAPPENS_H
