#include <iostream>
#include "antiVirus.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    u_long factors[4]={0,0,0,0};
    AntiVirus av = AntiVirus(10,10,0,5,factors);
    av.run();
    std::cout<<av;
    return 0;
}
