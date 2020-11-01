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
    for (size_t i = 0; i < g.districts.size(); ++i) {
        os << g.districts[i] << ": ";
        for (size_t j = 0; j < g.districts[i].getAssignedFields().size(); ++j) {
            os << g.districts[i].getAssignedFields()[j] << ", ";
        }
        os << std::endl;
    }
    os << "section3: districts assigned to a country" << std::endl;
    for (size_t i = 0; i < g.countries.size(); ++i) {
        os << g.countries[i] << ", ";
        for (size_t j = 0; j < g.countries[i].getAssignedDistrictIDs().size(); ++j) {
            os << g.countries[j].getAssignedDistrictIDs()[j] << ", ";
        }
        os << std::endl;
    }
    return os;
}