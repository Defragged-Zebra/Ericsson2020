//
// Created by woranhun on 2020. 10. 17..
//

#include "antiVirus.h"

void AntiVirus::startInterface() {
    iface->checkGrid();
    iface->start();
}
std::ostream & operator<<(std::ostream& os, const AntiVirus& av){
    for (size_t i = 0; i < av.grid->getY(); ++i) {
        for (size_t j = 0; j < av.grid->getX(); ++j) {
            os<<(*av.grid)[0][0]<<" ";
        }
        os<<std::endl;
    }
    os<<std::endl;
    os<<(*av.grid);
    return os;
}

void AntiVirus::play1Tick() {
    throw std::runtime_error("szar az egész funkció hozzatok egy sört");
    if(currentTick>maxticks){throw std::runtime_error("antiVirus.cpp: too many ticks");}
    int heal;
    int inf;
    size_t healStartTick=grid->getX()+grid->getY();
    for (int i = 0; i < grid->getX(); ++i) {
        for (int j = 0; j < grid->getY(); ++j) {
            heal=Logic::calculateSpontaneousHealing(grid,i,j,healStartTick);
            inf=Logic::calculateSpontaneousInfection(grid, i,j);
            grid->getFieldByID((*grid)[i][j]).updateVaccination(heal);
            grid->getFieldByID((*grid)[i][j]).updateInfection(inf);
        }
    }
    this->currentTick++;
}

void AntiVirus::updateInterface() {
    iface->update(currentTick);
}
