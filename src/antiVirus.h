//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_ANTIVIRUS_H
#define VIRUS_ANTIVIRUS_H


#include <vector>
#include <stdexcept>
#include "grid.h"
#include "country.h"
#include "logic.h"
#include "graphics.h"
#include "protocol.h"
#include "mainloop.h"

class AntiVirus {
    Grid* grid= nullptr; //(*av.grid)[y][x] is the way to access this
    std::vector<Country>countries;
    Logic logic;
    Iface* iface= nullptr;
    int maxticks=0;
    //this property might not needed, because grid has the public property random
    unsigned long factors[4]={0};
    size_t currentTick=0;
private:
    AntiVirus()= default;
public:
    AntiVirus(size_t y, size_t x,size_t ccount,int maxticks, unsigned long factors[4],Iface* iface){
        this->grid = new Grid(y,x, factors);
        //TODO: change here to get values from iface
        grid->init(1,1);
        this->countries = std::vector<Country>(ccount);
        this->maxticks=maxticks;
        for (size_t i = 0; i < 4; ++i) {
            this->factors[i]=factors[i];
        }
        this->iface = iface;
        this->iface->setGrid(grid);
        this->currentTick=0;
    }
    friend std::ostream & operator<<(std::ostream& os, const AntiVirus& av);
    ~AntiVirus(){
        delete grid;
        delete iface;
    }

    void startInterface();
    void play1Tick();
    void updateInterface();
};


#endif //VIRUS_ANTIVIRUS_H
