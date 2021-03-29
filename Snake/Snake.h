#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "Coords.h"

class Snake {
public:
	Snake(unsigned x, unsigned y);

	void addLimb();

	unsigned getX();
	unsigned getY();
	
	std::vector<Coords> getLimbs();

	/* Movement */
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();


private:
	std::vector<Coords> limbs;
	void update(unsigned x, unsigned y);
};

#endif