//
// Created by woranhun on 2020. 11. 01..
//

#ifndef VIRUS_UTILS_H
#define VIRUS_UTILS_H

#include <stdexcept>
#include <climits>
#include <vector>

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
        static size_t gridHeight,gridWidth;
    public:
        Point() = default;

        explicit Point(size_t fieldID){
            this->y = fieldID / gridWidth; //integer division is a design choice
            this->x = fieldID - (gridWidth * this->y);
        }

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
        bool operator<(const Point& p)const{
            if(this->getY()==p.getY())
                return this->getX() < p.getX();
            else
                return this->getY() < p.getY();
        }

        [[nodiscard]] bool withinBounds()const{
            return !(
                    this->getX()<0
                || this->getY()<0
                || this->getX() > gridWidth - 1
                || this->getY() > gridHeight - 1);
        }

        [[nodiscard]] size_t getY() const { return y; }

        [[nodiscard]] size_t getX() const { return x; }

        [[nodiscard]] static size_t getGridWidth(){return gridWidth;}
        [[nodiscard]] static size_t getGridHeight(){return gridHeight;}
        static void setGridWidth(size_t w){gridWidth=w;}
        static void setGridHeight(size_t h){gridHeight=h;}

        void setX(const size_t _x) { this->x = _x; }

        void setY(const size_t _y) { this->y = _y; }
        [[nodiscard]] std::vector<Point> getNeighbours()const;

        [[nodiscard]] bool isBorder()const{
            return this->getY()==0 or
            this->getX()==0 or
            this->getY()==Utils::Point::getGridHeight()-1 or
            this->getX()==Utils::Point::getGridWidth()-1;
        }
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

        //TODO: tanuljunk mar meg irni bazmeg: getCountyID
        [[nodiscard]] size_t getCounrtyID() const { return this->countryID; }
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

        [[nodiscard]] int getChangeInVaccines() const { return changeInVaccines; };

        [[nodiscard]] double getProfitabilityIndex() const { return profitabilityIndex; }

        [[nodiscard]] size_t getDistrictID() const { return districtID; }

        //bool operator<(const ScoreHolder &sc) const { return this->profitabilityIndex < sc.profitabilityIndex; }

        [[nodiscard]] bool profIndex(const ScoreHolder &sc)const{return this->profitabilityIndex < sc.profitabilityIndex;}
        [[nodiscard]] bool totalHealing(const ScoreHolder &sc)const{return this->vaccinesNeededForTotalHealing < sc.vaccinesNeededForTotalHealing;}
        bool operator==(ScoreHolder sc) const;
        bool operator <(ScoreHolder sc) const;

    };
    class Compare{
    public:
        class ProfIndex{
        public:
            bool operator()(const ScoreHolder &sc1,const ScoreHolder &sc2)const{
                return sc1.getProfitabilityIndex() < sc2.getProfitabilityIndex();
            }
        };
        class TotalHealing{
        public:
            bool operator()(const ScoreHolder &sc1,const ScoreHolder &sc2)const{
                return sc1.getVaccinesNeededForHealing() > sc2.getVaccinesNeededForHealing();
            }
        };
        class ProducedVaccines{
        public:
            bool operator()(const ScoreHolder &sc1,const ScoreHolder &sc2)const{
                return sc1.getChangeInVaccines() < sc2.getChangeInVaccines();
            }
        };
        class MinimalVaccinesToHeal{
        public:
            bool operator()(const ScoreHolder &sc1,const ScoreHolder &sc2)const{
                return sc1.getVaccinesNeededForHealing() < sc2.getVaccinesNeededForHealing();
            }
        };
    };

};

typedef Utils::Point Point;
typedef Utils::VaccineData VaccineData;
typedef Utils::Compare Compare;
typedef Utils::ScoreHolder ScoreHolder;

#endif //VIRUS_UTILS_H
