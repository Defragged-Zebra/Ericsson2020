//
// Created by lukac on 10/18/2020.
//

#include "lifehappens.h"

int LifeHappens::calculateSpontaneousHealing(Field field, int currentTick, int healStartTick, unsigned long random1) {
    throw std::runtime_error("under implementation");
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

int LifeHappens::calculateSpontaneousInfection(Grid *grid, int currentTick, unsigned long random2, unsigned long random3,
                                               unsigned long random4) {
    if (sectorIsClear) {
        return 0;
    } else {
        //curr_tick: ticks elapsed
        //A második véletlen faktor 10-zel való osztási maradéka + 10 darab előző vírusterjedés átlaga az adott cellán.
        int c = min(random2 % 10 + 10, currentTick);
        int b = avg(i =[1..c], infection(curr_tick - i, coord));
        int sum = 0;
        for (c in{[coordX, coordY], [coordX-1, coordY],[coordX, coordY+1],[coordX-1, coordY],[coordX, coordY-1]}) {
            if (/*boundary check fails*/)
                { continue; }
            int dist = distance(coord, c);
            //Az átfertőződési mutatók kiszámolása előtt a t átfertőződési hajlandóságot generáljuk
            // a harmadik véletlen faktor 7-tel való osztási maradéka + 3 -mal
            int t = random3 % 7 + 3;
            //Ehhez hozzáadva az adott cella és a szomszédjainak az átfertőződési mutatóját.
            a = b + sum(c = [coord, neighbours(coord)];
            //Az átfertőződési mutatókat akkor adjuk össze a hely populációs különbségéből adódó,
            //1-3 érték közé beszorított fertőzési lehetőséggel,
            //ha az előző körös fertőzöttség nagyobb, mint a hajlandóság és a távolság szorzata.
            if (tick_info[curr_tick - 1, c].infection_rate /*A.K.A: previous infection rate of the cell*/ > dist * t) {
                //"beszorított átfertőzési mutató"
                d = std::clamp(start_info[coord].population - start_info[c].population, 0, 2) + 1;
                sum += d;
            } else {
                continue;
            }
        }
        //Az így eddig kiszámolt összeget megszorozzuk a negyedik véletlen faktor 25-tel való osztási maradéka + 50-nel,
        //és az egészet leosztjuk 100-al, majd vesszük a felső egészrészét.
        return ceil(a * (factor4_next() % 25 + 50) / 100.0);
    }
}
int LifeHappens::distance(coord, c) {
    //A távolság helyben 0, megegyező kerületben 1, egyébként 2
    if (c.district != coord.district)
        {return 2;}
    else{
        if (coord==c){
            return 0;
        }
        else{
            return 1;
        }
    }
}