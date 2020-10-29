#include <iostream>
#include "antiVirus.h"
#include "examples.h"


void debug() {
    u_long factors[4] = {1569741360, 1785505948, 516548029, 1302116447};
    Random rd = Random(factors);
    size_t i = 0;
    int index = 2;
    do {
        std::cout << "i= " << i << " \tfactor2: " << rd.getFactor(2) << "    \tfactor3: " << rd.getFactor(3)
                  << "   \tfactor4: " << rd.getFactor(4) << std::endl;
        rd.next(2);
        rd.next(3);
        rd.next(4);
        ++i;
    } while (rd.getFactor(2) != 6014329);
    std::cout << "i= " << i << " \tfactor2: " << rd.getFactor(2) << "    \tfactor3: " << rd.getFactor(3)
              << " \tfactor4: " << rd.getFactor(4) << std::endl;
    //std::cout << "i= " << i << " factor: " << rd.getFactor(index) << std::endl;
    /*do  {
        ++i;
    } while (rd.next(index) != 413011888); //6014329);
    std::cout << "i= " << i << std::endl;*/

}

int main() {
    //debug();
    //return 0;
    std::stringstream ss = ex1();
    //std::stringstream ss = ex2();
    for (int i = 0; i < 44; ++i) {
        ss << "REQ 1 " << i << " 0" << std::endl;
        ss << "." << std::endl;
    }
    //AntiVirus av = AntiVirus(new Protocol(std::cin, std::cout, std::cerr));
    AntiVirus av = AntiVirus(new Protocol(ss, std::cout, std::cerr));
    av.startInterface();

    return 0;
}