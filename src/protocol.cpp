//
// Created by woranhun on 2020. 10. 17..
//

#include "protocol.h"

void Protocol::start() {
    init();
    initValues();
}

void Protocol::initValues() {
    std::string line;
    while (std::getline(is, line)) {
        os<<line<<std::endl;
    }
}

void Protocol::sendDebugMsg(const std::string &msg) {
    ers << msg << std::endl;
}
void Protocol::init() {
    os << "START " << teamToken << std::endl << "." << std::endl;
}
void Protocol::init(int seed) {
    os << "START " << teamToken << " " << seed << std::endl << "." << std::endl;
}
