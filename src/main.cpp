#include <iostream>
#include "antiVirus.h"



void debug() {
    u_long factors[4] = {1569741360, 1785505948, 516548029, 1302116447};
    Random rd = Random(factors);
    size_t i = 0;
    int index=3;
    for (int j = 0; j < 24; ++j) {
        //rd.next(index);
    }
    std::cout<<rd.getFactor(index)<<std::endl;
    bool overrun[4]={false,false, false, false};
    do  {
        std::cout << "i= " << i <<" factor: "<<rd.getFactor(index)<< std::endl;
        ++i;
    } while (rd.next(index) != 413011888); //6014329);
    std::cout << "i= " << i <<" factor: "<<rd.getFactor(index)<< std::endl;
    do  {
        ++i;
    } while (rd.next(index) != 413011888); //6014329);
    std::cout << "i= " << i << std::endl;

}


int main() {
    //debug();
    //return 0;
    std::stringstream ss;
    ss << "START 1 44 1"<< std::endl;
    ss << "FACTORS 1569741360 1785505948 516548029 1302116447"<< std::endl;
    ss << "FIELDS 6 4" << std::endl;
    ss << "FD 0 0 1 0 5" << std::endl;
    ss << "FD 0 1 1 1 1" << std::endl;
    ss << "FD 0 2 1 0 5" << std::endl;
    ss << "FD 0 3 3 0 5" << std::endl;
    ss << "FD 1 0 0 0 5" << std::endl;
    ss << "FD 1 1 1 0 4" << std::endl;
    ss << "FD 1 2 3 2 1" << std::endl;
    ss << "FD 1 3 3 0 2" << std::endl;
    ss << "FD 2 0 0 1 1" << std::endl;
    ss << "FD 2 1 2 0 5" << std::endl;
    ss << "FD 2 2 3 0 5" << std::endl;
    ss << "FD 2 3 5 0 5" << std::endl;
    ss << "FD 3 0 0 0 2" << std::endl;
    ss << "FD 3 1 2 1 1" << std::endl;
    ss << "FD 3 2 2 0 4" << std::endl;
    ss << "FD 3 3 5 2 1" << std::endl;
    ss << "FD 4 0 0 0 5" << std::endl;
    ss << "FD 4 1 2 0 5" << std::endl;
    ss << "FD 4 2 5 0 4" << std::endl;
    ss << "FD 4 3 5 0 3" << std::endl;
    ss << "FD 5 0 4 0 5" << std::endl;
    ss << "FD 5 1 4 1 1" << std::endl;
    ss << "FD 5 2 4 0 2" << std::endl;
    ss << "FD 5 3 4 0 5" << std::endl;
    ss << "." << std::endl;
    for (int i = 0; i < 44; ++i) {
        ss << "REQ 1 "<<i<<" 0" << std::endl;
        ss << "." <<std::endl;
    }


    u_long factors[4] = {2, 3, 4, 6};
    AntiVirus av = AntiVirus(new Protocol(ss,std::cout,std::cerr));
    //int maxTicks=300;
    //AntiVirus av = AntiVirus(25, 25, 0, maxTicks, factors, new Graphics());
    av.startInterface();
    //std::cout << av;
//    for (int i = 0; i < maxTicks; ++i) {
//        //district clear/not clear checker function is not created yet
//        av.play1Tick();
//        av.updateInterface();
//    }

    return 0;
}