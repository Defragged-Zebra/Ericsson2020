//
// Created by woranhun on 2020. 10. 17..
//

#include "field.h"

std::ostream& operator<<(std::ostream & os, const Field& f){
    os << f.getFieldID();
    return os;
}
