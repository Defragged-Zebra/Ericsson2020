//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_LOGIC_H
#define VIRUS_LOGIC_H

#include "mainloop.h"

class Logic:MainLoop {
public:
    Logic(){

    }

    void static simulateTO(int gameID, int tickID, int countryID);
};


#endif //VIRUS_LOGIC_H
