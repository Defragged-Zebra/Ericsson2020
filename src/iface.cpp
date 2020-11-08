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
    size_t numberOfDistricts = 0;
    size_t district;
    int infRate, population, tmp2;

    //according to input from ericsson
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            ss.clear();
            std::getline(is, line);
            ss << line;
            ss >> tmp >> tmp2 >> tmp2 >> district >> infRate >> population;
            if (district > numberOfDistricts) numberOfDistricts = district;
            grid->addField(Field(fieldID, district, infRate, 0, population,
                                 storedValsCnt));
            grid->setGridFieldID(y, x, fieldID++);
        }
    }
    //create the districts
    for (size_t i = 0; i < numberOfDistricts + 1; ++i) {
        grid->addDistrict(District(i, std::vector<Field *>(), false));
    }
    //TODO ez itt mi a fasz?
    //district update
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
//            ers << "Field - y: " << y << "\tx: " << x << "\tAssigned district: "
//                      << grid->getDistrictByPoint(Point(y, x)) << std::endl;
//            grid->getDistrictByPoint(Point(y, x)).addAssignedField(&grid->getFieldByPoint(Point(y, x)));

        }
    }
    setGrid(grid);

}

void Iface::start() {
    std::string line;
    while (std::getline(is, line)) {
        if (line == ".\r" or line == "\r" or line ==".") { // Lécci hadd működjön linuxon is :(
            continue;
        } else if (line == "SUCCESS") {
            Iface::sendDebugMsg("SUCCESS");
            break;
        } else if (line.find("WRONG") != std::string::npos) {
            Iface::sendDebugMsg(line);
            break;
        } else if (line.find("FAILED") != std::string::npos) {
            Iface::sendDebugMsg(line);
            break;
            //this else is never reached
        } else if (line.find("WARN") != std::string::npos) {
            Iface::sendDebugMsg(line);
            throw std::runtime_error("Beszoptuk a faszt!!444!!!");
        } else if (line.find("REQ") != std::string::npos) {
            Iface::round(line);
        } else {
            Iface::sendDebugMsg("Miafasz történt?");
            Iface::sendDebugMsg("[DEBUG] Hibát okozta\"" + line + "\" [DEBUG VEGE]");
            throw std::runtime_error("Miafasz történt?");
        }
    }
}

void Iface::requestLEGACY(std::string &line) {
    std::string tmp;
    std::stringstream ss;
    ss << line;
    int _gameID, tickID, countryID;
    ss >> tmp >> _gameID >> tickID >> countryID;
    Logic::simulateTO(_gameID, tickID, countryID);

    this->displayCurrentRound(_gameID, tickID, countryID);
}

void Iface::round(std::string &line) {
    std::string tmp;
    std::stringstream ss;
    ss << line;
    int _gameID, tickID, countryID;
    ss >> tmp >> _gameID >> tickID >> countryID;

    //Process input values
    tmp = "";
    while (std::getline(is, tmp)) {
        Iface::sendDebugMsg("[NOTIFY] " + tmp);
        if (tmp.find("WARN") != std::string::npos) {
            Iface::sendDebugMsg(line);
            throw std::runtime_error("We've fucked it up!!444!!!");
        }
        if (tmp != ".\r" and tmp !=".")continue; //Márk basszam meg, ez most linuxon nem jó...
        else break;
    }


    Logic::simulateTO(_gameID, tickID, countryID);

    //Send result back
    os << "RES " << _gameID << " " << tickID << " " << countryID << std::endl;
    std::vector<VaccineData> back;// Ha hozzá nyúlsz letöröm a kezed!!!!
    back = Logic::calculateBackVaccines(back, tickID);
    for (auto &i : back) {
        os << "BACK " << i.getY() << " " << i.getX() << " " << i.getVaccines() << std::endl;
    }
    std::vector<VaccineData> put;// Ha hozzá nyúlsz letöröm a kezed!!!!
    put = Logic::calculatePutVaccines(put, tickID);
    for (auto &i : back) {
        os << "PUT " << i.getY() << " " << i.getX() << " " << i.getVaccines() << std::endl;
    }

    this->displayCurrentRound(_gameID, tickID, countryID);

}
