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
        size_t y{}, x{};
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
        size_t countryID{};
    public:
        VaccineData() = default;

        VaccineData(const Point &gc, int vaccines, size_t countryID) {
            this->gridCell = gc;
            this->vaccines = vaccines;
            this->countryID = countryID;
        }

        VaccineData(size_t y, size_t x, int vaccines, size_t countryID) {
            this->gridCell = Point(y, x);
            this->vaccines = vaccines;
            this->countryID = countryID;
        }

        VaccineData &operator=(const VaccineData &bd) {
            if (this != &bd) {
                this->gridCell = bd.gridCell;
                this->vaccines = bd.vaccines;
                this->countryID = bd.countryID;
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

        [[nodiscard]] size_t getCountyID() const { return this->countryID; }
    };

    class ScoreHolder {
        size_t districtID;
        int changeInVaccines = INT_MIN;
        double profitabilityIndex;
        int vaccinesNeededForTotalHealing = INT_MAX;
    public:
        explicit ScoreHolder(size_t districtID) {
            profitabilityIndex = changeInVaccines / (double) vaccinesNeededForTotalHealing;
            this->districtID = districtID;
        }

        ScoreHolder(int changeInVaccines, int vaccinesNeededForTotalHealing, size_t districtID) {
            this->changeInVaccines = changeInVaccines;
            profitabilityIndex = changeInVaccines / (double) vaccinesNeededForTotalHealing;
            this->vaccinesNeededForTotalHealing = vaccinesNeededForTotalHealing;
            this->districtID = districtID;
        }

        [[nodiscard]] int getVaccinesNeededForHealing() const { return vaccinesNeededForTotalHealing; }

        [[nodiscard]] int ChangeInVaccines() const { return changeInVaccines; };

        [[nodiscard]] double getProfitabilityIndex() const { return profitabilityIndex; }

        [[nodiscard]] size_t getDistrictID() const {return districtID;}

        bool operator<(ScoreHolder& sc) const;
    };
};

typedef Utils::Point Point;
typedef Utils::VaccineData VaccineData;

#endif //VIRUS_UTILS_H
