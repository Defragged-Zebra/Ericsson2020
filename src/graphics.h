//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_GRAPHICS_H
#define VIRUS_GRAPHICS_H

#include "iface.h"

class Graphics : public Iface {
public:
    Graphics() = delete;

    Graphics(std::istream &is, std::ostream &os, std::ostream &ers) : Iface(is, os, ers) {}

    Graphics(const Graphics &p) = delete;

    Graphics &operator=(const Graphics &g) = delete;

    void start() override;

    void request(std::string &line);

    void terminalGraphicsRun(size_t gameID, size_t tickID, size_t countryID);
};


#endif //VIRUS_GRAPHICS_H
