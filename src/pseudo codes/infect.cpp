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
        c=min(factor2() % 10 + 10, curr_tick);
        b = avg(i = [1 .. c], infection(curr_tick - i, coord));
        t = factor3() % 7 + 3;
        a=b+sum(c = [coord, neighbours(coord)]; t = factor3() % 7 + 3, tick_info[curr_tick-1, c].infection_rate >
        (start_info[coord].district != start_info[c].district ? 2 :
        coord != c ? 1 : 0) * t ? clamp(start_info[coord].population - start_info[c].population, 0, 2) + 1 : 0);
        return ceil(a * (factor4_next() % 25 + 50) / 100.0);

    }
}

