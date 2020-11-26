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
    std::vector<std::vector<size_t>> grid{};
    std::vector<Country> countries{};
    std::vector<District *> districts{};
    std::vector<Field *> fields{};
    size_t width{};
    size_t height{};
    size_t currentTick = 0;
    bool clear = false;
    std::vector<std::vector<int>> allVaccinations;
public:

    Grid() = default;

    Grid(const Grid &g) {
        *this = g;
    }

    Grid &operator=(const Grid &g) {
        if (this != &g) {
            this->width = g.width;
            this->height = g.height;
            this->currentTick = g.currentTick;
            this->random = g.random;
            this->grid = g.grid;
            this->fields.clear();
            this->allVaccinations = g.allVaccinations;
            for (auto f:g.fields) {
                this->fields.push_back(new Field(*f));
            }
            this->districts.clear();
            for (auto d:g.districts) {
                auto *tmp = new District(*d);
                tmp->clearAssignedFields();
                this->districts.push_back(tmp);
            }

            for (size_t y = 0; y < this->getHeight(); ++y) {
                for (size_t x = 0; x < this->getWidth(); ++x) {
                    Point center(y, x);
                    std::vector<Point> coordinates = center.getNeighbours();
                    District centerDistrict = this->getDistrictByPoint(center);
                    this->getDistrictByPoint(Point(y, x)).addAssignedField(&this->getFieldByPoint(Point(y, x)));
                }
            }
            this->countries = g.countries;
        }
        return *this;
    }

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
            this->allVaccinations = g->allVaccinations;
        }
        return *this;
    }

    Utils::Random random;

    Grid(size_t height, size_t width, uint64_t seeds[4]) {
        this->height = height;
        this->width = width;
        random = Utils::Random(seeds);
        grid = std::vector<std::vector<size_t>>();
        allVaccinations = {};
        grid.reserve(height);
        allVaccinations.reserve(height);
        for (size_t i = 0; i < height; ++i) {
            std::vector<size_t> sor = std::vector<size_t>(width);
            grid.push_back(sor);
            //push_back copies
            std::vector<int> sor2 = std::vector<int>(width);
            allVaccinations.push_back(sor2);
        }
        /*
       int i = 0;
       int j=0;
       std::cerr << "grid constructor" << std::endl;
       for (auto it:allVaccinations) {
           for (auto it2:it) {
               std::cerr << "AllVac: " << i  <<" " <<j<< std::endl;
               i++;
               j++;
           }
       }*/
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

    void updateAllVaccination(const Point &p, int v) {
        if (!p.withinBounds()) {
            throw std::runtime_error("updateAllVaccination");
        }
        allVaccinations[p.getY()][p.getX()] = v;
    }

    int getAllVaccination(const Point &p) {
        /*
        std::cerr << "checkpoint 4.1.2.2.1" << std::endl;
        std::cerr << "Y: " << p.getY() << std::endl;
        std::cerr << "X: " << p.getX() << std::endl;
        auto deb1 = allVaccinations[p.getY()][p.getX()];
        std::cerr << "checkpoint 4.1.2.2.1" << std::endl;
        return deb1;
         */
        return allVaccinations[p.getY()][p.getX()];
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
        else {
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

    [[nodiscard]] Point getPointByFieldID(size_t ID) const {
        return Point(ID);
    }

    [[nodiscard]] std::vector<Field *> getNeihboursOfField(size_t FieldID) const {
        std::vector<Field *> retVals;
        std::vector<Point> neighbourPoints = this->getPointByFieldID(FieldID).getNeighbours();
        for (const auto &p: neighbourPoints) {
            if (p.withinBounds())retVals.push_back(this->fields[this->grid[p.getY()][p.getX()]]);
        }
        return retVals;

    }

    [[nodiscard]] inline Point getCoordinatesByID(size_t ID) const { return getPointByFieldID(ID); }

    [[nodiscard]] std::vector<Field *> getNeihboursOfField(const Field *f) const {
        std::vector<Field *> retVals;
        std::vector<Point> neighbourPoints = this->getPointByFieldID(f->getFieldID()).getNeighbours();
        for (const auto &p: neighbourPoints) {
            if (p.withinBounds())retVals.push_back(this->fields[this->grid[p.getY()][p.getX()]]);
        }
        return retVals;

    }

    int calculateDistrictProductionCapacity(size_t countryID, const District &district);

    void updateClearByFieldCheck() {
        bool allClear = true;
        for (auto field:fields) {
            allClear &= field->isClear();
        }
        clear = allClear;
    }

    [[nodiscard]] bool isClear() const { return clear; }

    void getNotVaccinatedFields(size_t countryID, std::map<size_t, std::set<Point>> notVaccinatedFields) const {
        for (auto d:districts) {
            for (auto f:d->getAssignedFields()) {
                if (f->getStoredVaccines()[countryID] == 0)
                    notVaccinatedFields[d->getDistrictID()].insert(getCoordinatesByID(f->getAssignedDistrictID()));
            }
        }
    }

    ~Grid() {
        for (const auto &field:fields) {
            delete field;
        }
        for (const auto &district:districts) {
            delete district;
        }
    }

    int calculateDistrictProductionCapacityLEGACYv2(size_t countryID, const District &district);
};


#endif //VIRUS_GRID_H
