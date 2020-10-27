#include <iostream>
#include "antiVirus.h"
#include "mainloop.h"

void debug() {
    u_long factors[4] = {1569741360, 1785505948, 4, 6};
    Random rd = Random(factors);
    size_t i = 0;
    do  {
        ++i;
    } while (rd.next(2) != 1022357306);
    std::cout << "i= " << i << std::endl;
}


int main() {
    debug();
    std::stringstream ss;
    ss << "Alma"; //test simulation
    u_long factors[4] = {2, 3, 4, 6};
    //AntiVirus av = AntiVirus(10,10,0,5,factors,new Protocol(ss,std::cout,std::cerr));
    int maxTicks = 100;
    AntiVirus av = AntiVirus(5, 5, 0, maxTicks, factors, new Graphics());
    av.startInterface();
    //std::cout << av;
    for (int i = 0; i < maxTicks; ++i) {
        //district clear/not clear checker function is not created yet
        av.play1Tick();
        av.updateInterface();
    }

    return 0;
}