//
// Created by woranhun on 2020. 11. 01..
//

#ifndef VIRUS_UTILS_H
#define VIRUS_UTILS_H

#include <stdexcept>
#include <climits>

class Utils {
public:
    class Random {
    private:
        uint64_t factor[4]{};
    public:
        Random() = default;

        explicit Random(const uint64_t seeds[4]);

        Random(const Random &r) {
            *this = r;
        }

        Random &operator=(const Random &r);

        //e.g. index 2 refers to factor2
        uint64_t next(int index);

        [[nodiscard]] uint64_t getFactor(int index) const;
    };

    class Point {
        size_t y, x;
    public:
        Point() = default;

        Point(size_t y, size_t x) {
            this->y = y;
            this->x = x;
        }

        Point &operator=(const Point &p) {
            if (this != &p) {
                this->y = p.y;
                this->x = p.x;
            }
            return *this;
        }

        Point(const Point &p) {
            *this = p;
        }

        bool operator==(const Point &p) const {
            return this->y == p.y && this->x == p.x;
        }

        [[nodiscard]] size_t getY() const { return y; }

        [[nodiscard]] size_t getX() const { return x; }

        void setX(const size_t _x) { this->x = _x; }

        void setY(const size_t _y) { this->y = _y; }
    };

    class VaccineData {
        Point gridCell{};
        int vaccines{};
    public:
        VaccineData() = default;

        VaccineData(const Point &gc, int vaccines) {
            this->gridCell = gc;
            this->vaccines = vaccines;
        }

        VaccineData(size_t y, size_t x, int vaccines) {
            this->gridCell = Point(y, x);
            this->vaccines = vaccines;
        }

        VaccineData &operator=(const VaccineData &bd) {
            if (this != &bd) {
                this->gridCell = bd.gridCell;
                this->vaccines = bd.vaccines;
            }
            return *this;
        }

        VaccineData(const VaccineData &bd) {
            *this = bd;
        }

        [[nodiscard]] Point getPoint() const { return this->gridCell; }

        [[nodiscard]] int getVaccines() const { return this->vaccines; }

        [[nodiscard]] size_t getX() const { return this->gridCell.getX(); }

        [[nodiscard]] size_t getY() const { return this->gridCell.getY(); }
    };

    class ScoreHolder {
        int changeInProducedVaccines;
        int changeInDefenseVaccines;
        double score = 0;
        double profitabilityIndex;
        int vaccinesNeededForTotalHealing = 0;
    public:
        ScoreHolder() {
            changeInDefenseVaccines = INT_MAX;
            changeInProducedVaccines = 0;
            profitabilityIndex = changeInProducedVaccines / (double) changeInDefenseVaccines;
        }

        ScoreHolder(int changeInProducedVaccines, int changeInDefenseVaccines, int vaccinesNeededForTotalHealing) {
            this->changeInDefenseVaccines = changeInDefenseVaccines;
            this->changeInProducedVaccines = changeInProducedVaccines;
            profitabilityIndex = changeInProducedVaccines / (double) changeInDefenseVaccines;
            this->vaccinesNeededForTotalHealing = vaccinesNeededForTotalHealing;
        }

        void setVaccinesNeededForHealing(int value) {
            vaccinesNeededForTotalHealing = value;
        }

        [[nodiscard]] int getVaccinesNeededForHealing() const { return vaccinesNeededForTotalHealing; }

        [[nodiscard]] int ChangeInProducedVaccines() const { return changeInProducedVaccines; };

        [[nodiscard]] int ChangeInDefenseVaccines() const { return changeInDefenseVaccines; }

        void updateScore(double newScore) { score = newScore; }

        [[nodiscard]] double getProfitabilityIndex() const { return profitabilityIndex; }
    };
};

typedef Utils::Point Point;
typedef Utils::VaccineData VaccineData;

#endif //VIRUS_UTILS_H
