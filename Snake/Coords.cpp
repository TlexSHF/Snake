#include "Coords.h"

namespace snake {

	Coords::Coords(unsigned x, unsigned y) : x(x), y(y) {}

	unsigned Coords::getX() {
		return x;
	}

	unsigned Coords::getY() {
		return y;
	}

	void Coords::setCoords(unsigned x, unsigned y) {
		this->x = x;
		this->y = y;
	}

	bool Coords::operator==(const Coords& other) {
		return this->x == other.x && this->y == other.y;
	}
}
