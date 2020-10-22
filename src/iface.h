//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_IFACE_H
#define VIRUS_IFACE_H


#include "fileio.h"
#include "grid.h"
#include "iface.h"

class Iface {
protected:
    FileIO fileio;
    Grid* grid;
public:
    Iface(){
    }
    explicit Iface(Grid* g){
        this->grid = g;
    }
    Iface(const Iface& iface){
        *this= iface;
    }
    Iface& operator=(const Iface& iface){
        if(this != &iface){
            this->fileio=iface.fileio;
            this->grid = iface.grid;
        }
        return *this;
    }
};


#endif //VIRUS_IFACE_H
