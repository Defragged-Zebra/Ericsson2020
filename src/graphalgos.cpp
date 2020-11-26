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

    pq.emplace(0, g->getFieldByPoint(start).getFieldID());
    dst[g->getFieldByPoint(start).getFieldID()] = 0;
    while (!pq.empty()) {
        Point u = g->getPointByFieldID(pq.top().second);
        if (u == goal)break;
        pq.pop();
        //std::vector<Point> p;
        //u.getNeighbours(p);
        Field &f = g->getFieldByPoint(u);
        for (auto &v:f.getNeig()) {
            int weight = g->getFieldByID(v).vaccinesToPutMinimal(countryID);
            if (dst[v] > dst[g->getFieldByPoint(u).getFieldID()] + weight) {

                dst[v] = dst[g->getFieldByPoint(u).getFieldID()] + weight;
                parent[v] = g->getFieldByPoint(u).getFieldID();
                pq.emplace(dst[v], v);

            }
        }
    }
    std::vector<Point> path;
    result = std::pair(path, calcPath(parent, path, g->getFieldByPoint(start).getFieldID(),
                                      g->getFieldByPoint(goal).getFieldID()));
}


void GraphAlgos::dijkstraLEGACY(const Point &start, const Point &goal, std::pair<std::vector<Point>, int> &result,
                                size_t _countryID) {
    if (start == goal) {
        result = std::pair(std::vector<Point>(), g->getFieldByPoint(goal).vaccinesToPutForTotalHealing(countryID));
        return;
    }
    this->countryID = _countryID;
    std::vector<size_t> dst(g->getWidth() * g->getHeight(), INT_MAX); //dst[fieldID]
    std::vector<size_t> parent(g->getWidth() * g->getHeight()); //parent[fieldID]
    std::priority_queue<nodeLEGACY, std::vector<nodeLEGACY>, std::greater<>> pq;

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

int calculateHvalAsManhattan(const Point& cur, const Point& goal){
    int x = (int)cur.getX();
    int y = (int)cur.getY();
    return abs(x-goal.getX())+abs(y-goal.getY());
}

void GraphAlgos::Astar(const Point &start, const Point &goal, std::pair<std::vector<Point>, int> &result,
                       size_t _countryID) {
    if (start == goal) {
        result = std::pair(std::vector<Point>(), g->getFieldByPoint(goal).vaccinesToPutForTotalHealing(countryID));
        return;
    }
    this->countryID = _countryID;
    std::vector<size_t> dst(g->getWidth() * g->getHeight(), INT_MAX); //dst[fieldID]
    std::vector<size_t> parent(g->getWidth() * g->getHeight()); //parent[fieldID]
    std::priority_queue<nodeLEGACY , std::vector<nodeLEGACY>, std::greater<>> pq;

    pq.emplace(0, start);
    dst[g->getFieldByPoint(start).getFieldID()] = 0;
    while (!pq.empty()) {
        Point u = pq.top().second;
        if (u == goal)break;
        pq.pop();

        for (auto &i:u.getNeighbours()) {
            const Point &v = i;
            int weight = g->getFieldByPoint(v).vaccinesToPutMinimal(countryID);
            if (dst[g->getFieldByPoint(v).getFieldID()] > dst[g->getFieldByPoint(u).getFieldID()] + weight+calculateHvalAsManhattan(v,goal)) {

                dst[g->getFieldByPoint(v).getFieldID()] = dst[g->getFieldByPoint(u).getFieldID()] + weight+calculateHvalAsManhattan(v,goal);
                parent[g->getFieldByPoint(v).getFieldID()] = g->getFieldByPoint(u).getFieldID();
                pq.emplace(dst[g->getFieldByPoint(v).getFieldID()], v);

            }
        }
    }
    std::vector<Point> path;
    result = std::pair(path, calcPath(parent, path, g->getFieldByPoint(start).getFieldID(),
                                      g->getFieldByPoint(goal).getFieldID()));

}
