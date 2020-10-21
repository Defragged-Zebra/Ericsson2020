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
    Iface()=default;
    Iface(const Iface& iface){
        *this= iface;
    }
    Iface& operator=(const Iface& iface){
        if(this != &iface){
            protocol=iface.protocol;
            fileio=iface.fileio;
            graphics=iface.graphics;
        }
        return *this;
    }
};


#endif //VIRUS_IFACE_H
