#ifndef SNAKE_H
#define SNAKE_H

#include <memory>
#include <vector>
#include "Coords.h"

class Snake {
public:
	Snake(unsigned x, unsigned y, size_t boardSize);

	void prepareNewLimb();
	void addLimb();

	/* Getters */
	unsigned getSize();
	int getX();
	int getY();
	bool hasNewLimb();
	
	std::vector<Coords> getLimbs();

	/* Movement */
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();


private:
	std::vector<Coords> limbs;
	std::unique_ptr<Coords> newLimb = nullptr;
	int boardSize;

	void update(int x, int y);
};

#endif