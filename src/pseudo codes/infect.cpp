//
// Created by lukac on 10/18/2020.
//

//random nums:
unsigned long factor1 -> factor4 = factor1.next(seed_from_server) //global variable or property of grid
//factor = factor * 48271 % 0x7fffffff
unsigned long factor1_next(){ //might be better w a class
    factor1 = factor1 * 48271 % 0x7fffffff;
    return factor1;
};
int healing() {
    int lastTicks = width + height;

    if (lastInfectionValues.len() < lastTicks) {
        return 0;
    } else {
        a = min(lastInfectionValues[lastInfectionValues.len() - lastTicks->lastInfectionValues.len()]);
        b = factor1_next() % 10;
        return floor(a * b / 20); //integer div is a design choice
    }
}

//infecting
//first element: infection(0, coord) => tick_info[0, coord].infection_rate > 0 ? 1 : 0

int infect (){
    if (sectorIsClear){
        return 0;
    } else{
        //curr_tick: ticks elapsed
        //A második véletlen faktor 10-zel való osztási maradéka + 10 darab előző vírusterjedés átlaga az adott cellán.
        c=min(factor2() % 10 + 10, curr_tick);
        b = avg(i = [1 .. c], infection(curr_tick - i, coord));
        sum=0;
        for(c in {[coordX,coordY],[coordX-1,coordY], [coordX, coordY+1], [coordX-1, coordY], [coordX,coordY-1]}) {
            if (/*boundary check fails*/)
                { continue; }

            dist = distance(coord, c);
            //Az átfertőződési mutatók kiszámolása előtt a t átfertőződési hajlandóságot generáljuk
            // a harmadik véletlen faktor 7-tel való osztási maradéka + 3 -mal
            t = factor3_next() % 7 + 3;
            //Ehhez hozzáadva az adott cella és a szomszédjainak az átfertőződési mutatóját.
            a = b + sum(c = [coord, neighbours(coord)];
            //Az átfertőződési mutatókat akkor adjuk össze a hely populációs különbségéből adódó,
            //1-3 érték közé beszorított fertőzési lehetőséggel,
            //ha az előző körös fertőzöttség nagyobb, mint a hajlandóság és a távolság szorzata.
            if (tick_info[curr_tick - 1, c].infection_rate /*A.K.A: previous infection rate of the cell*/ > dist * t){
                //"beszorított átfertőzési mutató"
                d = std::clamp(start_info[coord].population - start_info[c].population, 0, 2) + 1;
                sum += d;
            }
            else {
                continue;
            }
        }
        //Az így eddig kiszámolt összeget megszorozzuk a negyedik véletlen faktor 25-tel való osztási maradéka + 50-nel,
        //és az egészet leosztjuk 100-al, majd vesszük a felső egészrészét.
        return ceil(a * (factor4_next() % 25 + 50) / 100.0);

    }
}

int distance(coord,c) {
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
int new_values(){
    //Ez hozzáadódik a fertőzöttségi mutatóhoz
    //de a fertőzöttségi, illetve gyógyultsági mutató összege maximálisan 100 lehet.
    return std::clamp(infection+infect(),0,100); //this is the Field.currentInfectionValue
    return std::clamp(healed+healing(),0,100);  // this is the Field.vaccinationRate
}
