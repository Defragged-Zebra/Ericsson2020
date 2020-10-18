//
// Created by lukac on 10/18/2020.
//

#include "lifehappens.h"
#include <algorithm>

int LifeHappens::applySpontaneousHealing(Grid *grid, int currentTick, int healStartTick,) {
    throw std::runtime_error("random numbers not implemented yet");
    //healStartTick = width + height; -- it should be calculated further up for optimisation
    //currentTick = hanyadik tick van
    //Ha még nem értük el a width + height -edik kört, akkor 0
    if (currentTick < healStartTick) {
        return 0;
    } else {
        //Az előző tickek (pályaméret width + height darabszámú) fertőzöttségi mutatóinak minimuma szorozva az ...
        //a = min(lastInfectionValues[lastInfectionValues.len() - lastTicks->lastInfectionValues.len()]);
        a = std::min_element(field.getLastInfectionValues().begin(), field.getLastInfectionValues().end());
        //első véletlen faktor 10-zel való osztási maradékával (0-9)
        b = factor1_next() % 10;
        //Az eredmény osztva 20-al, és ennek az alsó egészrésze
        return floor(a * b / 20); //integer div is a design choice
    }
}
int LifeHappens::applySpontaneousInfection() {
    throw std::runtime_error("UML error");
}