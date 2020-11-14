//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_IFACE_H
#define VIRUS_IFACE_H

#include "grid.h"
#include "iface.h"
#include "logic.h"
#include "ai.h"
#include <ctype.h>

class Iface {
protected:
    Grid *grid;
    size_t gameID;
    size_t maxTickCount;
    size_t countriesCount;
    std::string teamToken = "eBPSHlkl";
    std::istream &is;
    std::ostream &os;
    std::ostream &ers;
public:
    Iface() = delete;

    explicit Iface(std::istream &is, std::ostream &os, std::ostream &ers) : is(is), os(os), ers(ers) {
        this->grid = nullptr;
    }

    explicit Iface(Grid *g, std::istream &is, std::ostream &os, std::ostream &ers) : is(is), os(os), ers(ers) {
        this->grid = g;
    }

    Iface &operator=(const Iface &iface) = delete;

    Iface(const Iface &iface) = delete;

    void checkGrid() {
        if (grid == nullptr)throw std::runtime_error("Iface: grid pointer is null");
    }

    virtual void setGrid(Grid *g) {
        grid = g;
        Logic::setGrid(g);
    }

    Grid *getGrid() const {
        return grid;
    }

    void initAntiVirus();

    void login() {
        os << "START " << teamToken << std::endl << "." << std::endl;
    }

    void login(int seed) {
        os << "START " << teamToken << " " << seed << std::endl << "." << std::endl;
    }

    void createGrid();

    size_t getGameID() const { return gameID; }

    size_t getMaxTick() const { return maxTickCount; }

    size_t getNumberOfCountries() const { return countriesCount; }

    virtual void start();

    virtual void displayCurrentRound(int gameID, int tickID, int countryID) {};

    void requestLEGACY(std::string &line);

    void sendDebugMsg(const std::string &msg) {
        ers << msg << std::endl;
    }

    virtual ~Iface() {}

    void round(std::string &line);
};


#endif //VIRUS_IFACE_H
