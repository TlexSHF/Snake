#include "Cell.h"

namespace snake {

    char Cell::getType() {
        return m_type;
    }

    void Cell::setType(char type) {
        if (m_type != type) {
            m_changed = true;
            m_type = type;
        } else {
            m_changed = false;
        }
    }

    bool Cell::hasChanged() {
        return m_changed;
    }

    std::ostream& operator<<(std::ostream& os, Cell& c) {
        return os << c.m_type;
    }
}
