//
// Created by woranhun on 2020. 10. 17..
//

#include "field.h"

std::ostream &operator<<(std::ostream &os, const Field &f) {
    os << f.getFieldID();
    return os;
}

void Field::updateVaccination(int healed) {
    //check for healing to not extend 100
    if (vaccinationRate + healed > 100) throw std::runtime_error("vaccinationRate + healed>100");
    vaccinationRate = std::min(vaccinationRate + healed, 100);
    infectionRate -= healed;
    if (infectionRate < 0) throw std::runtime_error("infectionRate <0");
}

void Field::updateInfection(int infected) {
    //check for infection to not extend 100
    int newInfectionRate = std::min(infected + infectionRate, 100 - vaccinationRate);
    lastInfectionValues.push_back(infected);
    lastInfectionRates.push_back(newInfectionRate);
    if (lastInfectionRates.size() > lastInfRateLen) { lastInfectionRates.pop_front(); }
    if (lastInfectionValues.size() > 19) { lastInfectionValues.pop_front(); }
    infectionRate = newInfectionRate;
}

