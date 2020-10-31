//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_GRAPHICS_H
#define VIRUS_GRAPHICS_H

#include "iface.h"

class Graphics : public Iface{
    std::string teamToken = "eBPSHlkl";
    std::istream& is;
    std::ostream& os;
    std::ostream& ers;
public:
    Graphics():is(std::cin),os(std::cout),ers(std::cerr), Iface() {}
    Graphics(std::istream& is,std::ostream& os,std::ostream& ers):is(is),os(os),ers(ers), Iface(){}
    Graphics(const Graphics &p)=delete;
    Graphics &operator=(const Graphics &g)=delete;
    void start() override;

    void initAntiVirus()override;

    void init();

    void init(int seed);

    void initValues();

    void setStart(std::string &line);

    void createGrid(std::string &line);

    void request(std::string &line);

    void terminalGraphicsRun(size_t gameID, size_t tickID, size_t countryID);
};


#endif //VIRUS_GRAPHICS_H
