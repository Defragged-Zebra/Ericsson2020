//
// Created by woranhun on 2020. 11. 02..
//

#ifndef VIRUS_VISUALIZATION_H
#define VIRUS_VISUALIZATION_H
#include "iface.h"
#include "window.h"

class Visualization : public Iface{
    Window ablak;
public:
    Visualization() = delete;

    Visualization(std::istream &is, std::ostream &os, std::ostream &ers) : Iface(is, os, ers) {}

    Visualization(const Visualization &p) = delete;

    Visualization &operator=(const Visualization &g) = delete;

    void setGrid(Grid* g)override{
        grid = g;
        Logic::setGrid(g);
        ablak.setGrid(grid);
    }
    void displayCurrentRound(int gameID, int tickID, int countryID) override;
};


#endif //VIRUS_VISUALIZATION_H
