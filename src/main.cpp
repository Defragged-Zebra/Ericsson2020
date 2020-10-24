#include <iostream>
#include "antiVirus.h"
#include "mainloop.h"

int main() {
    std::stringstream ss;
    ss << "Alma"; //test simulation
    u_long factors[4] = {2, 3, 4, 6};
    //AntiVirus av = AntiVirus(10,10,0,5,factors,new Protocol(ss,std::cout,std::cerr));
    AntiVirus av = AntiVirus(10, 10, 0, 100, factors, new Graphics());
    av.startInterface();
    //std::cout << av;
    for (int i = 0; i < 100; ++i) {
        //district clear/not clear checker function is not created yet
        av.play1Tick();
        av.updateInterface();
    }

    return 0;
}