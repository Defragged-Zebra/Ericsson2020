//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_GRID_H
#define VIRUS_GRID_H
#include <vector>
#include <stdexcept>
#include "field.h"
#include "random.h"
#include "country.h"
#include "district.h"
#include <algorithm>

class Grid {
    std::vector<std::vector<size_t>> grid;
    std::vector<Country> countries;
    std::vector<District> districts;
    std::vector<Field> fields;
    size_t x;
    size_t y;
    size_t currentTick =0;
    Grid(){
        throw std::runtime_error("grid default ctr");
    }
    Grid(const Grid&){
        throw std::runtime_error("grid copy ctr");
    }
    Grid& operator=(const Grid&){
        throw std::runtime_error("grid operator=");
    }

public:
    Random random;
    Grid(size_t y, size_t x, unsigned long seeds[4]){
        this->x=x;
        this->y=y;
        random=Random(seeds);
        grid = std::vector<std::vector<size_t>>();
        grid.reserve(y);
        for (size_t i = 0; i < y; ++i) {
            std::vector<size_t> sor = std::vector<size_t>(x);
            grid.push_back(sor);
        }
        this->countries=std::vector<Country>();
        this->districts=std::vector<District>();
    }
    void init(size_t districtCount, size_t countryCount);
    inline std::vector<size_t> operator[](size_t i){return grid[i];};
    inline size_t getX() const {return x;} //grid[0].size();}
    inline size_t getY() const {return y;} //grid.size();}
    //TODO: refactor y-x stuff
    size_t transformCoordinateToID(size_t y, size_t x){
                return grid[y][x];
    }
    Country &getCountryByID(size_t ID){return countries[ID];}
    District &getDistrictByID(size_t ID){return districts[ID];}
    Field &getFieldByID(size_t ID){return fields[ID];}

    template<typename FUNC>
    FUNC executeOnEveryElement(FUNC func);

    friend std::ostream &operator<<(std::ostream &os, const Grid &g);
    void uploadGridWithFieldID(size_t y, size_t x, size_t fieldID);
    //WARNING: this might generate some problems if grid is not newly created
    void addField(const Field& newField);
    void addDistrict(const District& newDistrict);
    void addCountry(const Country& newCountry);
    size_t getCurrentTick() const;
    void IncreaseCurrentTick(){currentTick++;}
};


#endif //VIRUS_GRID_H
