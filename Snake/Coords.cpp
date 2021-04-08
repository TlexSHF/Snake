#include "Coords.h"

namespace snake {

	Coords::Coords(unsigned x, unsigned y) : m_x(x), m_y(y) {}

	unsigned Coords::getX() {
		return m_x;
	}

	unsigned Coords::getY() {
		return m_y;
	}

	void Coords::setCoords(unsigned x, unsigned y) {
		m_x = x;
		m_y = y;
	}

	bool Coords::operator==(const Coords& other) {
		return m_x == other.m_x && m_y == other.m_y;
	}
}
