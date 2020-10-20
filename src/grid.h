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

class Grid {
    std::vector<std::vector<Field>> grid;
    Random random;
    std::vector<Country> countries;
    std::vector<District> districts;
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
    Grid(size_t y, size_t x, unsigned long seeds[4]){
        random=Random(seeds);
        grid = std::vector<std::vector<Field>>();
        grid.reserve(y);
        for (size_t i = 0; i < y; ++i) {
            std::vector<Field> sor = std::vector<Field>(x);
            grid.push_back(sor);
        }
        this->countries=std::vector<Country>();
        this->districts=std::vector<District>();
        districts
    }
    inline std::vector<Field> operator[](size_t i){return grid[i];};
    inline size_t getX(){return grid[0].size();}
    inline size_t getY(){return grid.size();}

    template<typename FUNC>
    FUNC executeOnEveryElement(FUNC func);


};


#endif //VIRUS_GRID_H
