//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_PROTOCOL_H
#define VIRUS_PROTOCOL_H
#include <iostream>

#include "iface.h"


class Protocol: public Iface {
    std::string teamToken = "eBPSHlkl";
    std::istream& is;
    std::ostream& os;
    std::ostream& ers;
public:
    Protocol():is(std::cin),os(std::cout),ers(std::cerr), Iface() {}
    Protocol(std::istream& is,std::ostream& os,std::ostream& ers):is(is),os(os),ers(ers), Iface(){}
    Protocol(const Protocol &p)=delete;
    Protocol &operator=(const Protocol &g)=delete;
    void init();

    void init(int seed);

    void sendDebugMsg(const std::string &msg);

    void initValues();

    void start() override;

    void setStart(std::string &line);

    void initAntiVirus() override;

    void createGrid(std::string &line);

    void request(std::string &line);

    void currentResult(int gameID, int tickID, int countryID);
};


#endif //VIRUS_PROTOCOL_H
