//
// Created by woranhun on 2020. 11. 13..
//

#ifndef VIRUS_GRAPHALGOS_H
#define VIRUS_GRAPHALGOS_H


#include "utils.h"
#include "field.h"
#include "grid.h"
#include "logic.h"

class GraphAlgos {
    typedef std::pair<int, Point> node;
    typedef std::pair<int, Point> nodeLEGACY;
    Grid *g = Logic::getGrid();
    size_t countryID;
public:

    int calcPath(std::vector<size_t> &parent, std::vector<Point> &result, size_t startFieldID, size_t endFieldID);

    void dijkstra(const Point &start, const Point &goal, std::pair<std::vector<Point>, int> &result, size_t _countryID);

    void dijkstraLEGACY(const Point &start, const Point &goal, std::pair<std::vector<Point>, int> &result,
                        size_t _countryID);
};


#endif //VIRUS_GRAPHALGOS_H
