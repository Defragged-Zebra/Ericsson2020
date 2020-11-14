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
    std::vector<District *> districts;
    std::vector<Field *> fields;
    size_t width;
    size_t height;
    size_t currentTick = 0;
    bool clear = false;

public:
    Grid() = delete;

    Grid(const Grid &g) = delete;

    Grid &operator=(const Grid *g) {
        if (this != g) {
            this->width = g->width;
            this->height = g->height;
            this->currentTick = g->currentTick;
            this->random = g->random;
            this->grid = g->grid;
            this->fields = g->fields;
            this->districts = g->districts;
            this->countries = g->countries;
        }
        return *this;
    }

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
        this->districts = std::vector<District *>();
        currentTick = 0;
        Point::setGridHeight(this->height);
        Point::setGridWidth(this->width);
    }

    //soronként töltjük fel a gridet(sorfolytonosan)
    std::vector<size_t> operator[](size_t i) { return grid[i]; }

    [[nodiscard]] size_t getWidth() const { return width; } //grid[0].size();}
    [[nodiscard]] size_t getHeight() const { return height; } //grid.size();}

    Country &getCountryByID(size_t ID) { return countries[ID]; }

    District &getDistrictByID(size_t ID) { return *districts[ID]; }

    District &getDistrictByPoint(const Point &p) {
        return getDistrictByID(getFieldByPoint(p).getAssignedDistrictID());
    }

    Field &getFieldByID(size_t ID) { return *fields[ID]; }

    Field &getFieldByPoint(const Point &p) {
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
    void addField(Field *newField) {
        fields.push_back(newField);
    }

    void addDistrict(District *newDistrict) {
        districts.push_back(newDistrict);
    }

    void addCountry(const Country &newCountry) {
        auto it = std::find_if(countries.begin(), countries.end(), [newCountry](Country const &obj) {
            return obj.getCountryID() == newCountry.getCountryID();
        });
        if (it == countries.end()) countries.push_back(newCountry);
        else{
            countries[newCountry.getCountryID()].setTotalProductionCapacity(newCountry.getTotalProductionCapacity());
            countries[newCountry.getCountryID()].setReserveVaccines(newCountry.getReserveVaccines());
        }
    }

    [[nodiscard]] size_t getCurrentTick() const {
        return currentTick;
    }

    [[nodiscard]] size_t getNumberOfDistricts() const {
        return districts.size();
    }

    void IncreaseCurrentTick() { currentTick++; }

    size_t numberOfDistricts() { return districts.size(); }

    [[nodiscard]] Point getPointByFieldID(size_t ID) const;
    [[nodiscard]] std::vector<Field*> getNeihboursOfField(size_t FieldID) const{
        std::vector<Field*> retVals;
        std::vector<Point> neighbourPoints = this->getPointByFieldID(FieldID).getNeighbours();
        for(auto p: neighbourPoints){
            if(p.withinBounds())retVals.push_back(this->fields[this->grid[p.getY()][p.getX()]]);
        }
        return retVals;

    }
    [[nodiscard]] std::vector<Field*> getNeihboursOfField(const Field* f) const{
        std::vector<Field*> retVals;
        std::vector<Point> neighbourPoints = this->getPointByFieldID(f->getFieldID()).getNeighbours();
        for(auto p: neighbourPoints){
            if(p.withinBounds())retVals.push_back(this->fields[this->grid[p.getY()][p.getX()]]);
        }
        return retVals;

    }

    int calculateChangeInProducedVaccinesByHealingDistrict(size_t countryID, const District &district);

    void updateClearByFieldCheck() {
        bool allClear = true;
        for (auto field:fields) {
            allClear &= field->isClear();
        }
        clear = allClear;
    }

    bool isClear() { return clear; }

    ~Grid() {
        for (const auto field:fields) {
            delete field;
        }
        for (const auto district:districts) {
            delete district;
        }
    }

};


#endif //VIRUS_GRID_H
