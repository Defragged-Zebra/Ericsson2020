//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_GRID_H
#define VIRUS_GRID_H
#include <vector>
#include <stdexcept>
#include "field.h"

class Grid {
    std::vector<std::vector<Field>> grid;
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
    Grid(size_t y, size_t x){
        grid = std::vector<std::vector<Field>>();
        grid.reserve(y);
        for (size_t i = 0; i < y; ++i) {
            std::vector<Field> sor = std::vector<Field>(x);
            grid.push_back(sor);
        }
    }
    std::vector<Field> operator[](size_t i){
        return grid[i];
}
    size_t getX(){return grid[0].size();}
    size_t getY(){return grid.size();}


};


#endif //VIRUS_GRID_H
