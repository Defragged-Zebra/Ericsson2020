//
// Created by lukac on 10/29/2020.
//

private byte getInfection(int tick) {
    if (tickData[tick].getRemaining() == 0) return 0;
    if (infectionAmounts[tick] != null) return infectionAmounts[tick];
    if (tick == 0) {
        infectionAmounts[tick] = (byte)((tickData[0].getInfectionRate() > 0) ? 1 : 0);
        return infectionAmounts[tick];
    }
    int count = (int) Math.min(MainClass.getRandom(1) % 10 + 10, tick);
    double sum = 0;
    for (int i = 1; i <= count; i++) {
        sum += getInfection(tick - i);
    }
    double averageInfection = sum / count;
    double s = 0;
    double t = MainClass.getRandom(2) % 7 + 3;
    for (District neighbour : neighbours) {
        double spreadChance = t;
        if (id != neighbour.id) {
            spreadChance *= 2;
        }

        if (neighbour.getInfectionRate(tick - 1) > spreadChance) {
            s += Math.min(2, Math.max(0, populationRate - neighbour.populationRate)) + 1;
        }
    }
    if (getInfectionRate(tick - 1) > 0) {
        s += 1;
    }

    byte spread = (byte) Math.ceil((averageInfection + s) * (MainClass.getRandom(3) % 25 + 50) / 100.0);
    infectionAmounts[tick] = spread;
    return spread;
}

public void spreadDisease(int tick) {
    byte spreadAmount = getInfection(tick);
    spreadAmount = (byte) Math.min(tickData[tick].getRemaining(), spreadAmount);
    tickData[tick].infect(spreadAmount);
}