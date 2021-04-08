#include "Snake.h"

namespace snake {

	Snake::Snake(unsigned x, unsigned y, size_t boardSize) :
		m_boardSize(boardSize) {

		m_limbs.emplace_back(x, y);
		//limbs.emplace_back(x - 1, y);
	}

	void Snake::update(int x, int y) {
		//Will only update if atleast one of the coordinates differ from previous
		if (x != m_limbs[0].getX() || y != m_limbs[0].getY()) {

			size_t size = m_limbs.size();
			for (int i = size - 1; i > 0; i--) {
				//Setting tail parts with the coords of the one before
				m_limbs[i].setCoords(
					m_limbs[i - 1].getX(),
					m_limbs[i - 1].getY()
				);
			}

			//Setting snake head
			//Cannot go outside of board border (0-size)
			if (x >= m_boardSize) {
				m_limbs[0].setCoords(0, y);
			} else if (x < 0) {
				m_limbs[0].setCoords(m_boardSize - 1, y);
			} else if (y >= m_boardSize) {
				m_limbs[0].setCoords(x, 0);
			} else if (y < 0) {
				m_limbs[0].setCoords(x, m_boardSize - 1);
			} else {
				m_limbs[0].setCoords(x, y);
			}
		}
	}

	std::vector<Coords> Snake::getLimbs() {
		//Returns only a copy of limbs
		// used for getting the coordinates
		return m_limbs;
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

	void Snake::prepareNewLimb() {
		//Generates a tail on the position of the last tail/head
		size_t size = m_limbs.size();
		m_newLimb = std::make_unique<Coords>(
			m_limbs[size - 1].getX(),
			m_limbs[size - 1].getY()
			);
	}

	void Snake::addLimb() {

		size_t size = m_limbs.size();
		if (hasNewLimb()) {
			m_limbs.emplace_back(
				m_newLimb->getX(),
				m_newLimb->getY()
			);
			m_newLimb = nullptr;
		}
	}

	unsigned Snake::getSize() {
		return m_limbs.size();
	}

	int Snake::getX() { //MAKE SURE THAT THERE ARE NO WAYS THE LIMBS VECTOR CAN BE 0
		return m_limbs[0].getX();
	}

	int Snake::getY() {
		return m_limbs[0].getY();
	}

	bool Snake::hasNewLimb() {
		return m_newLimb.get() != nullptr;
	}
}