#include "Cell.h"

#include <iostream> //take away

namespace snake {

    char Cell::getType() {
        return type;
    }

    void Cell::setType(char t) {
        if (type != t) {
            changed = true;
            type = t;
        } else {
            changed = false;
        }
    }

    bool Cell::hasChanged() {
        return changed;
    }
}
