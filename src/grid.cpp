//
// Created by woranhun on 2020. 10. 17..
//

#include "grid.h"

template<typename FUNC>
FUNC Grid::executeOnEveryElement(FUNC func) {
    for (size_t i = 0; i < getHeight(); ++i) {
        for (size_t j = 0; j < getWidth(); ++j) {
            func(grid[i][j]);
        }
    }
    return func;
}

std::ostream &operator<<(std::ostream &os, const Grid &g) {
    os << "section1: grid x-y to id codes:" << std::endl;
    os << "size of the grid: " << g.grid[0].size() << ", " << g.grid.size() << std::endl;
    for (size_t x = 0; x < g.grid[0].size(); ++x) {
        for (size_t y = 0; y < g.grid.size(); ++y) {
            os << g.grid[y][x] << ",";
        }
        os << std::endl;
    }
    os << "section2: fields assigned to a district" << std::endl;
    os << std::endl;
    for (const auto &district : g.districts) {
        os << district << ": ";
        for (auto it : district->getAssignedFields()) {
            os << it << ", ";
        }
        os << std::endl;
    }
    os << "section3: districts assigned to a country" << std::endl;
    for (const auto &countries : g.countries) {
        os << countries << ", ";
        for (auto it : countries.getAssignedDistricts()) {
            os << it << ", ";
        }
        os << std::endl;
    }
    return os;
}

int Grid::calculateDistrictProductionCapacity(size_t countryID, const District &district) {
    int changeInVaccines = 0;
    for (auto fieldPointer:district.getAssignedFields()) {
        //changed from 2 to 1 in 2.5, and also in v3.0
        changeInVaccines += 1;
        Point center = this->getPointByFieldID(fieldPointer->getFieldID());
        std::vector<Point> coordinates = center.getNeighbours();
        for (const auto &selected : coordinates) {

            if (!selected.withinBounds()) continue;
            if (this->getDistrictByPoint(selected) == this->getDistrictByPoint(center)) continue;
            int plusMinus;
            this->getDistrictByPoint(selected).isClear() ? plusMinus = +1 : plusMinus = -1;
            //changed in v2.5, if logic doesn't calculate right, check this first!!!
            //TPC - Kerületi védekezési vakcina 2.** A kerületek között a szabad áruforgalmat biztosítani kell, így az optimista `... területek 6 - start_info[coord].population különbségösszege, osztva 3-mal ...` sajnos megduplázódik, `... területek 6 - start_info[coord].population különbségösszege * 2, osztva 3-mal ...`, de ezáltal a mindig friss étel mindenkihez eljut.
            //Update for v3.0
            changeInVaccines += (int) (plusMinus * (ceil(
                    (6 - this->getFieldByPoint(selected).getPopulationDensity()) * 4 / (double) 5)));
        }
    }
    return changeInVaccines;
}

int Grid::calculateDistrictProductionCapacityLEGACYv2(size_t countryID, const District &district) {
    int changeInVaccines = 0;
    for (auto fieldPointer:district.getAssignedFields()) {
        changeInVaccines += 2;
        Point center = this->getPointByFieldID(fieldPointer->getFieldID());
        std::vector<Point> coordinates = center.getNeighbours();
        for (const auto &selected : coordinates) {
            if (!selected.withinBounds()) continue;
            if (this->getDistrictByPoint(selected) == this->getDistrictByPoint(center)) continue;
            int plusMinus;
            this->getDistrictByPoint(selected).isClear() ? plusMinus = +1 : plusMinus = -1;
            changeInVaccines += (int) (plusMinus * (ceil(
                    (6 - this->getFieldByPoint(selected).getPopulationDensity()) / (double) 3)));
        }
    }
    return changeInVaccines;
}
