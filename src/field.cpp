//
// Created by woranhun on 2020. 10. 17..
//

#include "field.h"

std::ostream &operator<<(std::ostream &os, const Field &f) {
    os << f.getFieldID();
    return os;
}

void Field::updateVaccination(int value) {
    //check for healing to not extend 100
    vaccinationRate = (std::min(vaccinationRate + value, 100));
}

void Field::updateInfection(int value) {
    //check for infection to not extend 100
    int newInfectionRate=std::min(infectionRate + value, 100);
    lastInfectionValues.push_back(value);
    lastInfectionRates.push_back(newInfectionRate);
    if (lastInfectionRates.size() > numberOfStoredPastValues) { lastInfectionRates.pop_front(); }
    if (lastInfectionValues.size() > numberOfStoredPastValues) { lastInfectionValues.pop_front(); }
    infectionRate = newInfectionRate;
}

