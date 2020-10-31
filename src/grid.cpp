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
            os << g.grid[y][x] << ",";
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

void Grid::uploadGridWithFieldID(const size_t fy, const size_t fx, const size_t fieldID) {
    grid[fy][fx] = fieldID;
}

void Grid::addField(const Field& newField) {
    fields.push_back(newField);
}

void Grid::addDistrict(const District& newDistrict) {
    districts.push_back(newDistrict);
}

void Grid::addCountry(const Country& newCountry) {
    countries.push_back(newCountry);
}

void Grid::init(size_t districtCount, size_t countryCount) {
    //TODO refactor it asap
    for (size_t i = 0; i < getX(); ++i) {
        for (size_t j = 0; j < getY(); ++j) {
            size_t ID = i * height + j;
            if ((i == 0 && j == 0) ) {
                addField(Field(ID, 0, 1, 0, 1, 20));
            } else if ((i==0&&j == 1)){
                addField(Field(ID, 0, 0, 0, 1, 20));
            }
            else{
                addField(Field(ID, 1, 0, 0, 1, 20));
            }
            grid[i][j] = ID;
        }
    }
    for (size_t i = 0; i < districtCount; ++i) {
        std::vector<size_t> fieldsToAssign = std::vector<size_t>();
        if (i == 0) {
            for (int j = 0; j < 2; ++j) {
                fieldsToAssign.push_back(j);
            }
        } else if (i == 1) {}
        for (int j = 2; j < width * height; ++j) {
            fieldsToAssign.push_back(j);
        }
        addDistrict(District(i, fieldsToAssign, false));
    }
    for (
            size_t i = 0;
            i < countryCount;
            ++i) {
        std::vector<size_t> districtsToAssign;
        districtsToAssign.push_back(0);
        districtsToAssign.push_back(1);
        addCountry(Country(i, districtsToAssign)
        );
    }
}

size_t Grid::getCurrentTick() const {
    return currentTick;
}



