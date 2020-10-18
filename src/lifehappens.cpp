//
// Created by lukac on 10/18/2020.
//

#include "lifehappens.h"
#include <queue>

int LifeHappens::calculateSpontaneousHealing(Grid *grid, int fieldCoordinateX, int fieldCoordinateY, int currentTick,
                                             int healStartTick, unsigned long random1) {
    Field field = (*grid)[fieldCoordinateX][fieldCoordinateY];
    //healStartTick = width + height; -- it should be calculated further up for optimisation
    //currentTick = hanyadik tick van
    //Ha még nem értük el a width + height -edik kört, akkor 0
    if (currentTick < healStartTick) {
        return 0;
    } else {
        //Az előző tickek (pályaméret width + height darabszámú) fertőzöttségi mutatóinak minimuma szorozva az ...
        //a = min(lastInfectionValues[lastInfectionValues.len() - lastTicks->lastInfectionValues.len()]);
        std::deque<int>::iterator tmp = std::min_element(field.getLastInfectionValues().begin(), field.getLastInfectionValues().end());
        int a = *tmp;
        //első véletlen faktor 10-zel való osztási maradékával (0-9)
        int b = random1 % 10;
        //Az eredmény osztva 20-al, és ennek az alsó egészrésze
        return floor(a * b / 20); //integer div is a design choice
    }
}

void LifeHappens::applySpontaneousHealing(int value, Grid *grid, int x,int y) {
    //check for healing to not extend 100
    int infectionValue=(*grid)[x][y].getLastInfectionValues()[-1];
    //this func should take care of the update of the que and stuff
    (*grid)[x][y].updateInfection(min(infectionValue+value,100));
}

int LifeHappens::calculateSpontaneousInfection(Grid *grid, int fieldCoordinateX, int fieldCoordinateY, int currentTick,
                                               unsigned long random2, unsigned long random3, unsigned long random4) {
    if (sectorIsClear) {
        return 0;
    } else {
        //curr_tick: ticks elapsed
        //A második véletlen faktor 10-zel való osztási maradéka + 10 darab előző vírusterjedés átlaga az adott cellán.
        int intervalToAverage = min(random2 % 10 + 10, currentTick);
        //int b = avg(i =[1..c], infection(curr_tick - i, coord));
        Field field = (*grid)[fieldCoordinateX][fieldCoordinateY];
        double b=0;
        std::deque<int> que=field.getLastInfectionValues();
        int size=que.size();
        for (int i = intervalToAverage; i < size; ++i) {
            b+=field.getLastInfectionValues()[i];
        }
        b = b/size;
        int sum = 0;
        int coordinates[5][2] = {{fieldCoordinateX,     fieldCoordinateY},
                                 {fieldCoordinateX - 1, fieldCoordinateY},
                                 {fieldCoordinateX,     fieldCoordinateY + 1},
                                 {fieldCoordinateX - 1, fieldCoordinateY},
                                 {fieldCoordinateX,     fieldCoordinateY - 1}};
        for (int i = 0; i < 5; ++i) {
            int cX = coordinates[i][0];
            int cY = coordinates[i][1];
            //boundary check
            if (cX < 0 || cY < 0 || cX > grid->getX() || cY > grid->getY()) {
                continue;
            }
            int dist = distance(grid, fieldCoordinateX, fieldCoordinateX, cX, cY);
            //Az átfertőződési mutatók kiszámolása előtt a t átfertőződési hajlandóságot generáljuk
            // a harmadik véletlen faktor 7-tel való osztási maradéka + 3 -mal
            int t = random3 % 7 + 3;


            //Az átfertőződési mutatókat akkor adjuk össze a hely populációs különbségéből adódó,
            //1-3 érték közé beszorított fertőzési lehetőséggel,
            //ha az előző körös fertőzöttség nagyobb, mint a hajlandóság és a távolság szorzata.
            Field cField = (*grid)[cX][cY];
            if (cField.getLastInfectionValues()[-2] > dist * t) {
                //"beszorított átfertőzési mutató"
                int d = std::clamp(field.getPopulationDensity() - cField.getPopulationDensity(), 0, 2) + 1;
                sum += d;
            } else {
                continue;
            }
        }
        //Ehhez hozzáadva az adott cella és a szomszédjainak az átfertőződési mutatóját.
        int a = b + sum;
        //Az így eddig kiszámolt összeget megszorozzuk a negyedik véletlen faktor 25-tel való osztási maradéka + 50-nel,
        //és az egészet leosztjuk 100-al, majd vesszük a felső egészrészét.
        return ceil(a * (random4 % 25 + 50) / 100.0);
    }
}

int LifeHappens::distance(Grid *grid, int x1, int y1, int x2, int y2) {
    //A távolság helyben 0, megegyező kerületben 1, egyébként 2
    if (c.district != coord.district) {
        return 2;
    } else {
        if (x1 == x2 && y1==y2) {
            return 0;
        } else {
            return 1;
        }
    }
}