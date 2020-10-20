//
// Created by woranhun on 2020. 10. 17..
//

#include "field.h"

std::ostream& operator<<(std::ostream & os,const Field& f){
    os << f.getFieldID();
    return os;
}
void Field::updateHealing(int value) {
    //refactor proposal: LifeHappens::applyHealing should be merged to Field::updateHealing
    vaccinationRate=value;
}
void Field::updateInfection(int value){
    //refactor proposal: LifeHappens::applyInfection should be merged to Field::updateInfection
    lastInfectionValues.push_back(value);
    // if we need to keep memory under control
    // imo, if we have a normal interface we should implement another func,
    // which is putting the last n values to a file regularly, and then deletes those
    //lastInfectionValues.pop_front();
    currentInfectionValue=value;
}