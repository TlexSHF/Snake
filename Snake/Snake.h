#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "Coords.h"

class Snake {
public:
	Snake(unsigned x, unsigned y, size_t boardSize);

	void addLimb();

	int getX();
	int getY();
	
	std::vector<Coords> getLimbs();

	/* Movement */
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();


private:
	std::vector<Coords> limbs;
	int boardSize;

	void update(int x, int y);
};

#endif