//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_GRAPHICS_H
#define VIRUS_GRAPHICS_H

#include "iface.h"

class Graphics : public Iface{
public:
    Graphics():Iface(){
    }
    Graphics(const Graphics& g):Iface(g){
        *this= g;
    }
    Graphics& operator=(const Graphics& g){
        if(this != &g){
        }
        return *this;
    }
    void start() override;
};


#endif //VIRUS_GRAPHICS_H
