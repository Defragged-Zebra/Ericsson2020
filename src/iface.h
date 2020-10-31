//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_IFACE_H
#define VIRUS_IFACE_H

#include "grid.h"
#include "iface.h"
#include "logic.h"

class Iface {
protected:
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
            this->grid = iface.grid;
            this->gameID = iface.gameID;
            this->maxTickCount = iface.maxTickCount;
            this->countriesCount = iface.countriesCount;
        }
        return *this;
    }
    void checkGrid(){
        if(grid==nullptr)throw std::runtime_error("Iface: grid pointer is null");
    }
    void setGrid(Grid* g){
        grid=g;
        Logic::setGrid(g);
    }
    Grid* getGrid()const{
        return grid;
    }
    size_t getGameID()const{return gameID;}
    size_t getMaxTick()const{return maxTickCount;}
    size_t getNumberOfCountries()const{return countriesCount;}
    virtual void start()=0;
    virtual void initAntiVirus()=0;
    virtual ~Iface(){}
};


#endif //VIRUS_IFACE_H
