//
// Created by woranhun on 2020. 10. 17..
//

#include "antiVirus.h"

void AntiVirus::run() {

}
std::ostream & operator<<(std::ostream& os, const AntiVirus& av){
    for (size_t i = 0; i < av.grid->getY(); ++i) {
        for (size_t j = 0; j < av.grid->getX(); ++j) {
            os<<(*av.grid)[0][0]<<" ";
        }
        os<<std::endl;
    }
    return os;
}
