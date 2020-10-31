//
// Created by woranhun on 2020. 10. 31..
//

#include "iface.h"

void Iface::initAntiVirus() {
    login();

    std::string line;
    std::getline(is, line);
    std::stringstream ss;
    size_t gameid, maxtickid, countriescount;
    std::string tmp;
    ss << line;
    ss >> tmp >> gameid >> maxtickid >> countriescount;
    this->gameID = gameid;
    this->maxTickCount = maxtickid;
    this->countriesCount = countriescount;

    Iface::createGrid();
    Logic::setGrid(grid);
}

void Iface::createGrid() {
    std::string line;
    std::getline(is, line);
    std::stringstream ss;
    ss << line;
    std::string tmp;
    ss >> tmp;
    uint64_t factors[4] = {0};
    for (uint64_t &factor : factors) {
        ss >> factor;
    }
    ss.clear();
    std::getline(is, line);
    //iy==rows, ix==columns
    size_t iy, ix;
    ss << line;
    ss >> tmp >> iy >> ix;
    grid = new Grid(iy, ix, factors);
    size_t fieldID = 0;
    size_t storedValsCnt = grid->getHeight() + grid->getWidth();
    size_t numberOfDisticts = 0;
    size_t district;
    int infRate, population, tmp2;
    //according to input from ericsson
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            ss.clear();
            std::getline(is, line);
            ss << line;
            ss >> tmp >> tmp2 >> tmp2 >> district >> infRate >> population;
            if (district > numberOfDisticts) numberOfDisticts = district;
            grid->addField(Field(fieldID, district, infRate, 0, population,
                                 std::max((int) storedValsCnt, 19)));
            grid->uploadGridWithFieldID(y, x, fieldID++);
        }
    }
    for (size_t i = 0; i < numberOfDisticts + 1; ++i) {
        grid->addDistrict(District(i, std::vector<size_t>(), false));
    }
}
void Iface::start() {
    std::string line;
    while (std::getline(is, line)) {
        if (line != ".") {
            Iface::request(line);
        }
    }
}
void Iface::request(std::string &line) {
    std::string tmp;
    std::stringstream ss;
    ss << line;
    int _gameID, tickID, countryID;
    ss >> tmp >> _gameID >> tickID >> countryID;
    Logic::simulateTO(_gameID, tickID, countryID);

    this->currentResult(_gameID, tickID, countryID);
}
