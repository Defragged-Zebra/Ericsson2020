#include <iostream>
#include <fstream>
#include "antiVirus.h"

//comment this line to exclude graphics
#define production

int main() {
//    debug3();
//    return 0;
//    std::stringstream ss = ex1();
//        for (int i = 0; i < 43; ++i) {
//        ex3->ss << "REQ 1 " << i << " 0" << std::endl;
//        ex3->ss << "." << std::endl;
//    }
    std::ifstream ifs;
    ifs.open("../testfiles/round2/visual.csv");
    //AntiVirus av = AntiVirus(new Protocol(std::cin, std::cout, std::cerr));
    //AntiVirus av = AntiVirus(new Graphics(ifs,std::cout,std::cout));
    //AntiVirus av = AntiVirus(new Protocol(ifs, std::cout, std::cout));
    AntiVirus av = AntiVirus(new Visualization(ifs, std::cout, std::cout));
    av.startInterface();

    return 0;
}