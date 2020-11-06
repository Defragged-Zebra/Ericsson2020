//
// Created by woranhun on 2020. 11. 02..
//

#ifndef VIRUS_VISUALIZATION_H
#define VIRUS_VISUALIZATION_H
#include "iface.h"
#include "draw.h"

class Visualization : public Iface{
public:
    Visualization() = delete;

    Visualization(std::istream &is, std::ostream &os, std::ostream &ers) : Iface(is, os, ers) {
        // window init comes here
    }

    Visualization(const Visualization &p) = delete;

    Visualization &operator=(const Visualization &g) = delete;


    void currentResult(int gameID, int tickID, int countryID)override{
        this->displayResult(gameID,tickID,countryID);
    }
    void displayResult(int gameID, int tickID, int countryID);
};


#endif //VIRUS_VISUALIZATION_H
