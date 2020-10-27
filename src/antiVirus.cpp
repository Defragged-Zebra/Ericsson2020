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
    if(currentTick>maxticks){throw std::runtime_error("antiVirus.cpp: too many ticks");}
    int heal;
    int inf;
    size_t healStartTick=grid->getX()+grid->getY();
    for (int i = 0; i < grid->getX(); ++i) {
        for (int j = 0; j < grid->getY(); ++j) {
            heal=MainLoop::calculateSpontaneousHealing(grid,i,j,currentTick,healStartTick,grid->random.next(1));
            inf=MainLoop::calculateSpontaneousInfection(grid, i,j,currentTick,grid->random.next(2),grid->random.next(3),grid->random.next(4));
            grid->getFieldByID((*grid)[i][j]).updateVaccination(heal);
            grid->getFieldByID((*grid)[i][j]).updateInfection(inf);
        }
    }
    currentTick++;
}

void AntiVirus::updateInterface() {
    iface->update(currentTick);
}
