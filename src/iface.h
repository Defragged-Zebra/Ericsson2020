//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_IFACE_H
#define VIRUS_IFACE_H


#include "protocol.h"
#include "fileio.h"
#include "graphics.h"
#include "grid.h"

class Iface {
    FileIO fileio;
    Grid* grid;
public:
    Iface(){
        throw std::runtime_error("Grid* cannot be null!!!");
    }
    explicit Iface(Grid* g){
        this->grid = g;
    }

};


#endif //VIRUS_IFACE_H
