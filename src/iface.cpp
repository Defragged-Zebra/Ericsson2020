//
// Created by woranhun on 2020. 10. 31..
//

#include "iface.h"

void Iface::initAntiVirus() {
#ifdef SEED_FIXED
    login(SEED);
#else
    login();
#endif
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
            grid->addField(new Field(fieldID, district, infRate, 0, population,
                                     storedValsCnt));
            grid->setGridFieldID(y, x, fieldID++);
        }
    }
    //create the districts
    //numberOfDistricts+1 because numberOfDistricts stores the max district ID
    for (size_t i = 0; i < numberOfDistricts + 1; ++i) {
        grid->addDistrict(new District(i, std::set<Field *>(), std::set<size_t>(), false));
    }
    //district update
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            Point center(y, x);
            std::vector<Point> coordinates = center.getNeighbours();
            District centerDistrict = grid->getDistrictByPoint(center);
            for (const auto &selected:coordinates) {
                if (!selected.withinBounds())continue;
                District &selectedDistrict = grid->getDistrictByPoint(selected);
                if (centerDistrict != selectedDistrict) {
                    centerDistrict.addNeighbourDistrict(selectedDistrict.getDistrictID());
                    selectedDistrict.addNeighbourDistrict(centerDistrict.getDistrictID());
                }
            }
            grid->getDistrictByPoint(Point(y, x)).addAssignedField(&grid->getFieldByPoint(Point(y, x)));
        }
    }
    for (size_t y = 0; y < grid->getHeight(); ++y) {
        for (size_t x = 0; x < grid->getWidth(); ++x) {
            std::vector<size_t> n;
            Field &f = grid->getFieldByPoint(Point(y, x));
            for (auto &p :Point(y, x).getNeighbours()) {
                f.neigh.push_back(grid->getFieldByPoint(p).getFieldID());
            }
        }
    }
    setGrid(grid);

}

void Iface::start() {
    ers << "checkpoint -1" << std::endl;
    std::string line;
    while (std::getline(is, line)) {
        if (line == ".\r" or line == "\r" or line == ".") {
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
        } else if (line.find("WARN") != std::string::npos) {
            Iface::sendDebugMsg(line);
        } else if (line.find("REQ") != std::string::npos) {
            Iface::round(line);
        } else {
            Iface::sendDebugMsg("Failed to process line:");
            Iface::sendDebugMsg("[DEBUG] Error caused by\"" + line + "\" [END OF DEBUG]");
        }
#ifndef PROD
        grid->updateClearByFieldCheck();
        if (grid->isClear()) {
            break;
        }
#endif
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
    ers << "[TICKID]: " << grid->getCurrentTick() << std::endl;
    ers << "checkpoint 0" << std::endl;
    std::string tmp;
    std::stringstream ss;
    ss << line;
    int _gameID, tickID, countryID;
    ss >> tmp >> _gameID >> tickID >> countryID;

    //Process input values
    tmp = "";
    while (std::getline(is, tmp)) {
        if (tmp == ".\r" or tmp == ".")break;
#ifdef VERBOSE
        Iface::sendDebugMsg("[NOTIFY] original line:" + tmp);
#endif
        ss.clear();

        //TODO: parse game-data here (test this before uncommenting)
        if (tmp.find("VAC") != std::string::npos) {
#ifdef INFO
            Iface::sendDebugMsg("[NOTIFY] " + tmp);
#endif
            int row, column, sum_pre_vaccine, vaccinated;
            ss << tmp;
            std::string barmi;
            ss >> barmi;
            ss >> row >> column >> sum_pre_vaccine >> vaccinated;
            ers << std::endl;
            ers << "row: " << row << std::endl;
            ers << "column: " << column << std::endl;
            ers << "sumpre: " << sum_pre_vaccine << std::endl;
            ers << "vaccinated: " << vaccinated << std::endl;
//            //variable healed should store the amount of newly healed people
//            sum_pre_vaccine -- mennyi vakcina van osszesen az összes országnak ::Vakcina beadás és gyógyulás kepletben az *m*
//            vaccinated: összesen mennyi vaknica adótdott be ::Vakcina beadás és gyógyulás kepletben az *X*
            int healed = vaccinated;
            grid->updateAllVaccination(Point(column, row), healed);
        }


        if (tmp.find("SAFE") != std::string::npos) {
#ifdef INFO
            Iface::sendDebugMsg("[NOTIFY] " + tmp);
#endif
            int healedDistrictID;
            ss << tmp;
            std::string strtmp;
            ss >> strtmp;
            ss >> countryID >> healedDistrictID;
            grid->getCountryByID(countryID).addAssignedDistrict(healedDistrictID);
            grid->getDistrictByID(healedDistrictID).setClear(true);
        } else if (std::isdigit(tmp[0])) {
#ifdef INFO
            Iface::sendDebugMsg("[NOTIFY] " + tmp);
#endif
            int _countryID, TPC, RV;
            ss << tmp;
            ss >> _countryID >> TPC >> RV;
            grid->addCountry(Country(_countryID, TPC, RV));
        }
    }
    ers << "checkpoint 1" << std::endl;
    Logic::simulateTO(_gameID, tickID, countryID);
    ers << "checkpoint 2" << std::endl;
    this->displayCurrentRound(_gameID, tickID, countryID);
    int numberOfVaccinesToDistribute = grid->getCountryByID(countryID).getReserveVaccines();
    AI::copyGrid(grid);
    std::vector<VaccineData> back; // don't change this
    ers << "checkpoint 3" << std::endl;
    back = AI::calculateBackVaccines(back, numberOfVaccinesToDistribute, countryID);

#ifdef INFO
    ers << "[INFO] Vaccines before ai decision: " << numberOfVaccinesToDistribute << std::endl;
#endif

    ers << "checkpoint 4 -- AI:put" << std::endl;
    std::vector<VaccineData> put; // don't change this
    put = AI::calculatePutVaccines(put, numberOfVaccinesToDistribute, countryID);
    ers << "checkpoint 5" << std::endl;
    Logic::simulateVaccination(back, put);

#ifdef INFO
    ers << "[INFO] Vaccines after ai decision: " << numberOfVaccinesToDistribute << std::endl;
    ers << "[INFO] Assigned districts: " << grid->getCountryByID(countryID).getAssignedDistricts().size()
        << std::endl;
#endif
    //ers<<"checkpoint 6"<<std::endl;

    //Send result back
    os << "RES " << _gameID << " " << tickID << " " << countryID << std::endl;
    for (auto &i : back) {
        os << "BACK " << i.getY() << " " << i.getX() << " " << i.getVaccines() << std::endl;
    }
    for (auto &i : put) {
        os << "PUT " << i.getY() << " " << i.getX() << " " << i.getVaccines() << std::endl;
    }
    os << "." << std::endl;
    //ers<<"checkpoint 7"<<std::endl;
}
