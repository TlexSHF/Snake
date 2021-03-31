#ifndef DIRECTION_H
#define DIRECTION_H

enum class Direction {
	up,
	down,
	right,
	left
};

inline Direction operator!(const Direction& d) {
	//Returns the opposite
	Direction opposite = Direction::up;
	switch (d) {
	case Direction::up:
		opposite = Direction::down;
		break;
	case Direction::down:
		opposite = Direction::up;
		break;
	case Direction::right:
		opposite = Direction::left;
		break;
	case Direction::left:
		opposite = Direction::right;
		break;
	}

	return opposite;
}

#endif