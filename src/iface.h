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
    size_t gameID;
    size_t maxTickCount;
    size_t countriesCount;
public:
    Iface(){ grid=nullptr; }
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
    void setGrid(Grid* g){
        grid=g;
    }
    void checkGrid(){
        if(grid==nullptr)throw std::runtime_error("Iface: grid pointer is null");
    }
    virtual void start()=0;
    virtual void update(size_t tickID) =0;
    virtual ~Iface(){}
    size_t getGameID(){return gameID;}
    size_t getMaxTick(){return maxTickCount;}
    size_t getNumberOfCountries(){return countriesCount;}

};


#endif //VIRUS_IFACE_H
