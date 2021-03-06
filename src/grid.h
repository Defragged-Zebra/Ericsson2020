//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_GRID_H
#define VIRUS_GRID_H

#include <vector>
#include <stdexcept>
#include "field.h"
#include "utils.h"
#include "country.h"
#include "district.h"
#include <algorithm>

class Grid {
    std::vector<std::vector<size_t>> grid;
    std::vector<Country> countries;
    std::vector<District> districts;
    std::vector<Field> fields;
    size_t width;
    size_t height;
    size_t currentTick = 0;

public:
    Grid() = delete;

    Grid(const Grid &) = delete;

    Grid &operator=(const Grid &) = delete;

    Utils::Random random;

    Grid(size_t height, size_t width, uint64_t seeds[4]) {
        this->height = height;
        this->width = width;
        random = Utils::Random(seeds);
        grid = std::vector<std::vector<size_t>>();
        grid.reserve(height);
        for (size_t i = 0; i < height; ++i) {
            std::vector<size_t> sor = std::vector<size_t>(width);
            grid.push_back(sor);
        }
        this->countries = std::vector<Country>();
        this->districts = std::vector<District>();
        currentTick = 0;
    }
    //soronként töltjük fel a gridet(sorfolytonosan)
    std::vector<size_t> operator[](size_t i) { return grid[i]; };

    [[nodiscard]] size_t getWidth() const { return width; } //grid[0].size();}
    [[nodiscard]] size_t getHeight() const { return height; } //grid.size();}

    Country &getCountryByID(size_t ID) { return countries[ID]; }

    District &getDistrictByID(size_t ID) { return districts[ID]; }
    District &getDistrictByPoint(const Point& p){
        return getDistrictByID(getFieldByPoint(p).getAssignedDistrictID());
    }

    Field &getFieldByID(size_t ID) { return fields[ID]; }
    Field &getFieldByPoint(const Point& p){
        return getFieldByID(grid[p.getY()][p.getX()]);
    }

    template<typename FUNC>
    FUNC executeOnEveryElement(FUNC func);
    //TODO újraírni a picsába
    friend std::ostream &operator<<(std::ostream &os, const Grid &g);

    //TODO kibaszni a picsába
    void setGridFieldID(size_t y, size_t x, size_t fieldID) {
        grid[y][x] = fieldID;
    }

    //WARNING: this might generate some problems if grid is not newly created
    void addField(const Field &newField) {
        fields.push_back(newField);
    }

    void addDistrict(const District &newDistrict) {
        districts.push_back(newDistrict);
    }

    void addCountry(const Country &newCountry) {
        countries.push_back(newCountry);
    }

    [[nodiscard]] size_t getCurrentTick() const {
        return currentTick;
    }

    void IncreaseCurrentTick() { currentTick++; }
};


#endif //VIRUS_GRID_H
