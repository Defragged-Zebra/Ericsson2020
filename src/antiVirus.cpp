//
// Created by woranhun on 2020. 10. 17..
//

#include "antiVirus.h"

void AntiVirus::startInterface() {
    iface->checkGrid();
    iface->start();
}

std::ostream &operator<<(std::ostream &os, const AntiVirus &av) {
    for (size_t i = 0; i < av.grid->getHeight(); ++i) {
        for (size_t j = 0; j < av.grid->getWidth(); ++j) {
            os << (*av.grid)[0][0] << " ";
        }
        os << std::endl;
    }
    os << std::endl;
    os << (*av.grid);
    return os;
}
