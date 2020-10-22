//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_PROTOCOL_H
#define VIRUS_PROTOCOL_H
#include <iostream>

#include "iface.h"


class Protocol: public Iface {
    std::string teamToken = "eBPSHlkl";
public:
    Protocol() : Iface() {
    }

    Protocol(const Protocol &p) {
        *this = p;
    }

    Protocol &operator=(const Protocol &g) {
        if (this != &g) {
            //fields come here
        }
        return *this;
    }

    void init() {
        std::cout << "START" << teamToken << std::endl << "." << std::endl;
    }

    void init(int seed) {
        std::cout << "START" << teamToken << " " << seed << std::endl << "." << std::endl;
    }

    void sendDebugMsg(const std::string &msg) {
        std::cerr << msg << std::endl;
    }

    void initValues() {
        std::string line;
        while (std::getline(std::cin, line)) {
        }
    }
};


#endif //VIRUS_PROTOCOL_H
