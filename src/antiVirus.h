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
private:
    AntiVirus()= default;
    public:
    AntiVirus(size_t y, size_t x){
        grid = new Grid(y,x);
    }
    void run();
    friend std::ostream & operator<<(std::ostream& os, const AntiVirus& av){
        for (size_t i = 0; i < av.grid->getY(); ++i) {
            for (size_t j = 0; j < av.grid->getX(); ++j) {
                os<<(*av.grid)[0][0]<<" ";
            }
            os<<std::endl;
        }
        return os;
    }
    ~AntiVirus(){
        delete grid;
    }
};


#endif //VIRUS_ANTIVIRUS_H
