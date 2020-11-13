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
    //ToDo Scale it down to the necessary size
    std::map<size_t ,std::pair<Point , int>> adjList{}; //adjacencies of the give PointID
    Grid* g = Logic::getGrid();
public:
    void createAdjlist() {
        for (size_t y = 0; y < g->getHeight(); ++y) {
            for (size_t x = 0; x < g->getWidth(); ++x) {
                Field& f = g->getFieldByPoint(Point(y,x));
                std::vector<Field*> neighbours = g->getNeihboursOfField(f.getFieldID());
                adjList.insert(std::pair<size_t ,std::pair<Point , int>>(f.getFieldID(),std::pair(Point(y,x),f.getCurrentInfectionRate())));
            }
        }

    }

    void dijkstra(const Point& start, const Point& goal, const std::pair<std::vector<Point>, int>& pair) {
        createAdjlist();
        std::vector<size_t> dst(adjList.size(),INFINITY);
        dst[g->getFieldByPoint(start).getFieldID()]=0;
        //ToDo in progress

    }
};


#endif //VIRUS_GRAPHALGOS_H
