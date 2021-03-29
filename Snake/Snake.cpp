#include "Snake.h"

Snake::Snake(unsigned x, unsigned y) {
	limbs.emplace_back(x, y);
	//limbs.emplace_back(x - 1, y); //are we gonna have two or nah?
}

void Snake::update(unsigned x, unsigned y) {
	//Will only update if atleast one of the coordinates differ from previous
	if (x != limbs[0].getX() || y != limbs[0].getY()) {

		size_t size = limbs.size();
		for (int i = size - 1; i > 0; i--) {
			//Setting parts with the coords of the one before
			limbs[i].setCoords(
				limbs[i - 1].getX(), 
				limbs[i - 1].getY()
			);
		}

		limbs[0].setCoords(x, y);
	}
}

std::vector<Coords> Snake::getLimbs() {
	//Returns only a copy of limbs
	// used for getting the coordinates
	return limbs;
}

void Snake::moveUp() {
	update(getX(), getY() - 1);
}

void Snake::moveDown() {
	update(getX(), getY() + 1);
}

void Snake::moveRight() {
	update(getX() + 1, getY());
}

void Snake::moveLeft() {
	update(getX() - 1, getY());
}

void Snake::addLimb() {
	//Generates a tail on the position of the last tail/head, 
	// then later moves on with the rest
	size_t size = limbs.size();
	limbs.emplace_back(
		limbs[size - 1].getX(),
		limbs[size - 1].getY()
	);
}

unsigned Snake::getX() { //MAKE SURE THAT THERE ARE NO WAYS THE LIMBS VECTOR CAN BE 0
	return limbs[0].getX();
}

unsigned Snake::getY() {
	return limbs[0].getY();
}