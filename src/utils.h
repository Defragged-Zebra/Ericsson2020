//
// Created by woranhun on 2020. 11. 01..
//

#ifndef VIRUS_UTILS_H
#define VIRUS_UTILS_H
#include <stdexcept>
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
    class Point{
        size_t y,x;
    public:
        Point()=default;
        Point(size_t y, size_t x){
            this->y=y;
            this->x=x;
        }
        Point& operator=(const Point& p){
            if(this!=&p){
                this->y=p.y;
                this->x=p.x;
            }
            return *this;
        }
        Point(Point& p){
            *this=p;
        }
        bool operator==(const Point& p) const{
            return this->y==p.y&&this->x==p.x;
        }
        [[nodiscard]] size_t getY()const{return y;}
        [[nodiscard]] size_t getX()const{return x;}
        void setX(const size_t _x){this->x=_x;}
        void setY(const size_t _y){this->y=_y;}
    };
};
typedef Utils::Point Point;

#endif //VIRUS_UTILS_H
