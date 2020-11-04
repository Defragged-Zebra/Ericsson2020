//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_PROTOCOL_H
#define VIRUS_PROTOCOL_H

#include <iostream>

#include "iface.h"


class Protocol : public Iface {
public:
    Protocol() = delete;

    Protocol(std::istream &is, std::ostream &os, std::ostream &ers) : Iface(is, os, ers) {}

    Protocol(const Protocol &p) = delete;

    Protocol &operator=(const Protocol &g) = delete;


    void displayCurrentRound(int gameID, int tickID, int countryID)override{
        this->displayResult(gameID,tickID,countryID);
    }
    void displayResult(int gameID, int tickID, int countryID);

};


#endif //VIRUS_PROTOCOL_H
