//
// Created by woranhun on 2020. 10. 17..
//

#include "grid.h"

template<typename FUNC>
FUNC Grid::executeOnEveryElement(FUNC func) {
    for (size_t i = 0; i < getY(); ++i) {
        for (size_t j = 0; j < getX(); ++j) {
            func(grid[i][j]);
        }
    }
    return func;
}

std::ostream &operator<<(std::ostream &os, const Grid &g) {
    os << "section1: grid x-y to id codes:" << std::endl;
    os << "size of the grid: " << g.grid[0].size() << ", " << g.grid.size() << std::endl;
    for (int x = 0; x < g.grid[0].size(); ++x) {
        for (int y = 0; y < g.grid.size(); ++y) {
            os << g.grid[x][y] << ", ";
        }
        os << std::endl;
    }
    os << "section2: fields assigned to a district" << std::endl;
    os << std::endl;
    //TODO: clear up this mess
    for (int i = 0; i < g.districts.size(); ++i) {
        os << g.districts[i] << ": ";
        for (int j = 0; j < g.districts[i].getAssignedFields.size(); ++j) {
            os<<g.districts[i].getAssignedFields[j]<<", "
        }
        os<<std::endl;
    }
    os << std::endl;
    os <<"section3: districts assigned to a country"<<std::endl;
    for (int i = 0; i < g.countries.size(); ++i) {
        os << g.countries[i] << ", ";
        for (int j = 0; j < g.countries[i].getAssignedDistrictIDs.size(); ++j) {
            os<<g.countries[j].getAssignedDistrictIDs[j]<<", "
        }
        os<<std::endl;
    }
    return os;
}
