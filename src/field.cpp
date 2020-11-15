//
// Created by woranhun on 2020. 10. 17..
//

#include "field.h"

//std::ostream &operator<<(std::ostream &os, const Field &f) {
//    os << f.getFieldID();
//    return os;
//}

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

void Field::updateRemainingVaccines(int vaccinated) {
    //m: used vaccines (thx ericsson for the straightforward variable naming)
    // a tartalék vakcinaszám csökkenni fog m = ceil(X / P) -nel.
    int m = std::ceil(vaccinated / (double) populationDensity);
    int n = 0;
    for (auto tmp:storedVaccines) {
        n += tmp.second;
    }
    if (n == 0) return;
    for (auto tmp:storedVaccines) {
        /*Országokra levetítve n1 n2 két ország vakcinaszáma n1 = floor(n1 * (n - m) / n)
        * és n2 = floor(n2 * (n - m) / n) lesz. */
        //if (n == 0) throw std::runtime_error("Mé vagy buzi?"); // #rememberthenegativehealing
        int usedVaccinesByIndividualCountry = floor((tmp.second) * (n - m) / (double) n);
        storedVaccines[tmp.first] = usedVaccinesByIndividualCountry;
    }
}

void Field::callBackVaccines(int vaccines, size_t countryID) {
    int &storedVacs = this->storedVaccines[countryID];
    if ((storedVacs - vaccines) < 0)
        throw std::runtime_error("Ez fos: storedVacs-vaccines<0, majd ha összeáll, akkor már szar lesz");
    storedVacs -= vaccines;
    if (storedVacs < 0)throw std::runtime_error("Ez fos: storedVacs-vaccines<0, majd ha összeáll, akkor már szar lesz");
}

void Field::pushVaccines(int vaccines, size_t countryID) {
    if (vaccines < 0) throw std::runtime_error("miafasz");
    this->storedVaccines[countryID] += vaccines;
}

