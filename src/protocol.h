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


    void start() override;

    void request(std::string &line);

    void currentResult(int gameID, int tickID, int countryID);

    void sendDebugMsg(const std::string &msg);
};


#endif //VIRUS_PROTOCOL_H
