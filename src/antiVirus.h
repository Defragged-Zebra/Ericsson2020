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

class AntiVirus {
    Grid* grid= nullptr; //(*av.grid)[y][x] is the way to access this
    Iface* iface= nullptr;
private:
    AntiVirus()= default;
public:

    explicit AntiVirus(Iface* iface){
        this->iface=iface;
        this->iface->initAntiVirus();
        this->grid = this->iface->getGrid();
    }
    friend std::ostream & operator<<(std::ostream& os, const AntiVirus& av);
    ~AntiVirus(){
        delete grid;
        delete iface;
    }

    void startInterface();
};


#endif //VIRUS_ANTIVIRUS_H
