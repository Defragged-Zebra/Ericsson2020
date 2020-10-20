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
#include "iface.h"

class AntiVirus {
    Grid* grid= nullptr; //(*av.grid)[y][x] is the way to access this
    std::vector<Country>coutries;
    Logic logic;
    Iface iface;
    int maxticks=0;
    u_long factors[4]={0};
private:
    AntiVirus()= default;
public:
    AntiVirus(size_t y, size_t x,size_t ccount,int maxticks,const u_long factors[4]){
        this->grid = new Grid(y,x);
        this->coutries = std::vector<Country>(ccount);
        this->maxticks=maxticks;git
        for (size_t i = 0; i < 4; ++i) {
            this->factors[i]=factors[i];
        }
        this->iface = Iface();
    }
    void run();
    friend std::ostream & operator<<(std::ostream& os, const AntiVirus& av);
    ~AntiVirus(){
        delete grid;
    }
};


#endif //VIRUS_ANTIVIRUS_H
