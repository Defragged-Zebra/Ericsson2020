//
// Created by lukac on 10/18/2020.
//

#include "lifehappens.h"

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
        int a = std::min_element(field.getLastInfectionValues().begin(), field.getLastInfectionValues().end());
        //első véletlen faktor 10-zel való osztási maradékával (0-9)
        int b = random1 % 10;
        //Az eredmény osztva 20-al, és ennek az alsó egészrésze
        return floor(a * b / 20); //integer div is a design choice
    }
}

void LifeHappens::applySpontaneousHealing(int value, Field field) {
    //check for healing to not extend 100
    throw error;
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

        int b = std::accumulate(field.getLastInfectionValues()[len - intervalToAverage],
                                field.getLastInfectionValues().end(), 0.0) /
                field.getLastInfectionValues().len();
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
            int dist = distance(fieldCoordinateX, fieldCoordinateX, cX, cY);
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

int LifeHappens::distance(coord, c) {
    //A távolság helyben 0, megegyező kerületben 1, egyébként 2
    if (c.district != coord.district) {
        return 2;
    } else {
        if (coord == c) {
            return 0;
        } else {
            return 1;
        }
    }
}