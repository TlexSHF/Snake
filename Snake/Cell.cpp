#include "Cell.h"

char Cell::getType() {
    return type;
}

char Cell::getPrevType() {
    return prevType;
}

void Cell::setType(char t) {
    prevType = type;
    type = t;
}