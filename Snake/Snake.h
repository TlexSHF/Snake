#ifndef SNAKE_H
#define SNAKE_H

#include <memory>
#include <vector>
#include "Coords.h"

namespace snake {

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

		std::vector<Coords> getLimbs(); //TODO: Might rename all "limbs" to "segments"

		/* Movement */
		void moveUp();
		void moveDown();
		void moveRight();
		void moveLeft();


	private:
		std::vector<Coords> m_limbs;
		std::unique_ptr<Coords> m_newLimb = nullptr;
		int m_boardSize;

		void update(int x, int y);
	};
}

#endif