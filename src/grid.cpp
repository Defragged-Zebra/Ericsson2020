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
            os << g.grid[x][y] << ",";
        }
        os << std::endl;
    }
    os << "section2: fields assigned to a district" << std::endl;
    os << std::endl;
    for (size_t i = 0; i < g.districts.size(); ++i) {
        os << g.districts[i] << ": ";
        for (int j = 0; j < g.districts[i].getAssignedFields().size(); ++j) {
            os << g.districts[i].getAssignedFields()[j] << ", ";
        }
        os << std::endl;
    }
    os << "section3: districts assigned to a country" << std::endl;
    for (size_t i = 0; i < g.countries.size(); ++i) {
        os << g.countries[i] << ", ";
        for (int j = 0; j < g.countries[i].getAssignedDistrictIDs().size(); ++j) {
            os << g.countries[j].getAssignedDistrictIDs()[j] << ", ";
        }
        os << std::endl;
    }
    return os;
}

void Grid::setCellIndex(size_t x, size_t y, size_t cellIndex) {
    grid[x][y] = cellIndex;
}

void Grid::addField(Field newField) {
    fields.push_back(newField);
}

void Grid::addDistrict(District newDistrict) {
    districts.push_back(newDistrict);
}

void Grid::addCountry(Country newCountry) {
    countries.push_back(newCountry);
}

void Grid::init(size_t districtCount, size_t countryCount) {
    for (size_t i = 0; i < getX(); ++i) {
        for (size_t j = 0; j < getY(); ++j) {
            addField(Field(i * y + j,-1,0,0,1,1));
        }
    }
    for (size_t i = 0; i < districtCount; ++i) {
        addDistrict(District(i,std::vector<size_t>(), true));
    }
    for (size_t i = 0; i < countryCount; ++i) {
        addCountry(Country(i,std::vector<size_t>()));
    }
}



