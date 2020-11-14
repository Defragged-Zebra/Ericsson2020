//
// Created by woranhun on 2020. 11. 13..
//

#include "graphalgos.h"

int
GraphAlgos::calcPath(std::vector<size_t> &parent, std::vector<Point> &result, size_t startFieldID, size_t endFieldID) {
    int dist = 0;
    if (parent[endFieldID] == startFieldID) {
        result.push_back(g->getPointByFieldID(endFieldID));
        //we want to completely heal the endpoint, so we put the vaccinesToPutForTotalHealing() there
        return dist + g->getFieldByID(endFieldID).vaccinesToPutForTotalHealing(countryID);
    }
    dist += calcPath(parent, result, startFieldID, parent[endFieldID]);
    result.push_back(g->getPointByFieldID(endFieldID));
    return dist + g->getFieldByID(endFieldID).vaccinesToPutMinimal(countryID);
}

void GraphAlgos::dijkstra(const Point &start, const Point &goal, std::pair<std::vector<Point>, int> &result,
                          size_t _countryID) {
    if (start == goal) {
        result = std::pair(std::vector<Point>(), g->getFieldByPoint(goal).vaccinesToPutForTotalHealing(countryID));
        return;
    }
    this->countryID = _countryID;
    std::vector<size_t> dst(g->getWidth() * g->getHeight(), INT_MAX); //dst[fieldID]
    std::vector<size_t> parent(g->getWidth() * g->getHeight()); //parent[fieldID]
    std::priority_queue<node, std::vector<node>, std::greater<>> pq;

    pq.emplace(0, start);
    dst[g->getFieldByPoint(start).getFieldID()] = 0;
    while (!pq.empty()) {
        Point u = pq.top().second;
        if (u == goal)break;
        pq.pop();

        for (auto &i:u.getNeighbours()) {
            const Point &v = i;
            int weight = g->getFieldByPoint(v).vaccinesToPutMinimal(countryID);
            if (dst[g->getFieldByPoint(v).getFieldID()] > dst[g->getFieldByPoint(u).getFieldID()] + weight) {

                dst[g->getFieldByPoint(v).getFieldID()] = dst[g->getFieldByPoint(u).getFieldID()] + weight;
                parent[g->getFieldByPoint(v).getFieldID()] = g->getFieldByPoint(u).getFieldID();
                pq.emplace(dst[g->getFieldByPoint(v).getFieldID()], v);

            }
        }
    }
    std::vector<Point> path;

    result = std::pair(path, calcPath(parent, path, g->getFieldByPoint(start).getFieldID(),
                                      g->getFieldByPoint(goal).getFieldID()));
}
