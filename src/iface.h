//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_IFACE_H
#define VIRUS_IFACE_H


#include "protocol.h"
#include "fileio.h"
#include "graphics.h"

class Iface {
    Protocol protocol;
    FileIO fileio;
    Graphics graphics;

public:
    Iface();
};


#endif //VIRUS_IFACE_H
