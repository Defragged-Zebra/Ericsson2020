//
// Created by woranhun on 2020. 11. 01..
//

#include "utils.h"

size_t Point::gridHeight = 0;
size_t Point::gridWidth = 0;

std::vector<Point> Utils::Point::getNeighbours() const {
    std::vector<Point> points = std::vector<Point>();
    points.emplace_back(Point(y + 1, x));
    points.emplace_back(Point(y, x + 1));
    points.emplace_back(Point(y - 1, x));
    points.emplace_back(Point(y, x - 1));
    return points;
}

Utils::Random::Random(const uint64_t seeds[4]) {
    for (int i = 0; i < 4; ++i) {
        factor[i] = seeds[i];
    }
}

Utils::Random &Utils::Random::operator=(const Random &r) {
    if (this != &r) {
        for (size_t i = 0; i < 4; ++i) {
            this->factor[i] = r.factor[i];
        }
    }
    return *this;
}

//e.g. index 2 refers to factor2
uint64_t Utils::Random::next(int index) {
    factor[index - 1] = factor[index - 1] * 48271UL % 0x7fffffffUL;
    return factor[index - 1];
}

uint64_t Utils::Random::getFactor(int index) const {
    return factor[index - 1];
}

bool Utils::ScoreHolder::operator==(ScoreHolder sc) const {
    if (this->districtID == sc.districtID)
        return true;
    return false;
}

bool Utils::ScoreHolder::operator<(ScoreHolder sc)const {
    return this->districtID<sc.districtID;
}
