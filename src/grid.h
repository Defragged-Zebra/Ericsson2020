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
    inline std::vector<size_t> operator[](size_t i){return grid[i];};
    inline size_t getX(){return grid[0].size();}
    inline size_t getY(){return grid.size();}
    size_t transformCoordinateToID(size_t x, size_t y){
        //design: 0   -> [0,0]   1   -> [0,1]  2   -> [0,2]  ...
        //        x   -> [1,0]   x+1 -> [1,1]  x+2 -> [1,2]  ...
        //        ...
        for (int i = 0; i < x; ++i) {
            for (int j = 0; j < y; ++j) {
                return i*y+j;
            }
        }
    }
    Country getCountryByID(size_t ID){return countries[ID];}
    District getDistrictByID(size_t ID){return districts[ID];}
    Field getFieldByID(size_t ID){return fields[ID];}

    template<typename FUNC>
    FUNC executeOnEveryElement(FUNC func);

    friend std::ostream &operator<<(std::ostream &os, const Grid &g);
    void setCellIndex(size_t x, size_t y, size_t cellIndex);
    //WARNING: this might generate some problems if grid is not newly created
    void addField(Field newField);
    void addDistrict(District newDistrict);
    void addCountry(Country newCountry);
};


#endif //VIRUS_GRID_H
