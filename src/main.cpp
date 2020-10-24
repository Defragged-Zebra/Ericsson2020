#include <iostream>
#include "antiVirus.h"

int main() {
    std::stringstream ss;
    ss << "Alma"; //test simulation
    u_long factors[4] = {0, 0, 0, 0};
    //AntiVirus av = AntiVirus(10,10,0,5,factors,new Protocol(ss,std::cout,std::cerr));
    AntiVirus av = AntiVirus(10, 10, 0, 5, factors, new Graphics());
    av.startInterface();
    std::cout << av;
    return 0;
}