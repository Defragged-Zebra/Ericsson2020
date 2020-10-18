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