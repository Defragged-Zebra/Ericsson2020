#include <iostream>
#include "antiVirus.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    AntiVirus av = AntiVirus(10,10,0,5);
    av.run();
    std::cout<<av;
    return 0;
}
