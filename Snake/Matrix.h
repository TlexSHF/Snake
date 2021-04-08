
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <array>
#include <vector>
#include "Cell.h"
#include "Coords.h"
#include "Snake.h"

namespace snake {

	template<const size_t size>
	class Matrix {
	public:
		Matrix();

		/* Getters */
		const std::array<std::array<Cell, size>, size> getLayout();
		unsigned getSpecialFruits();
		bool isGameOver();
		unsigned getScore();

		/* Movement */
		void moveSnakeUp();
		void moveSnakeDown();
		void moveSnakeRight();
		void moveSnakeLeft();

		/* Actions */
		void activateSpecialFruit();

		friend std::ostream& operator<<(std::ostream& os, Matrix<size>& m) {
			for (auto column : m.m_matrix) {
				for (auto cell : column) {
					if (cell.getType() == '0') {
						os << "  ";
					} else {
						os << cell << " ";
					}
				}
				os << std::endl;
			}
			return os;
		}

	private:
		std::array<std::array<Cell, size>, size> m_matrix;
		std::vector<Coords> m_walls;
		std::vector<Coords> m_tempWalls;
		Snake m_snake;
		Coords m_fruit;
		Coords m_specialFruit;
		unsigned m_specialFruits = 0;
		bool m_specialSnake = false;
		bool m_specialFruitOnTable = false;
		bool m_gameOver = false;
		unsigned m_timesSpecial = 0;

		/* Creations */
		void createWalls();
		void createTempWall();
		void spawnSpecialFruit();
		void newRdmCoords(Coords& c);
		bool spawnPossible(); 
		bool isNearSnakeHead(unsigned x, unsigned y);
		void breakTempWall(unsigned x, unsigned y);

		/* Updating */
		void updateBoard();
		void emptyBoard();
		void updateWalls();
		void updateTempWalls();
		void updateSnake();

		void setCellType(Coords c, char type);
		char getCellType(Coords c);
		void checkCollision();

		/* Actions */
		void eatFruit();
		void eatSpecial();
	};

	template<size_t size>
	inline Matrix<size>::Matrix() :
		m_matrix(),
		m_snake(4, 4, size), //THESE CAN NOT BE HARDCODED
		m_fruit(0, 0),
		m_specialFruit(0, 0) {

		srand(time(NULL)); //init psuedo random number

		emptyBoard();
		createWalls();

		newRdmCoords(m_snake.getLimbs()[0]); //TODO This doesn't do much it seems.
		newRdmCoords(m_fruit);
		setCellType(m_fruit, 'f');

		updateBoard();
	}

	template<size_t size>
	inline const std::array<std::array<Cell, size>, size> Matrix<size>::getLayout() {
		return m_matrix;
	}

	template<size_t size>
	inline bool Matrix<size>::isGameOver() {
		return m_gameOver;
	}

	template<size_t size>
	inline unsigned Matrix<size>::getSpecialFruits() {
		return m_specialFruits;
	}

	template<size_t size>
	inline unsigned Matrix<size>::getScore() {
		return m_snake.getSize() - 1;
	}

	template<size_t size>
	void Matrix<size>::updateBoard() {
		//placing possible new limb
		if (m_snake.hasNewLimb()) {
			m_snake.addLimb();
		}

		//Snake can only be special for a couple of moves
		//If special snake, it will not collide
		if (m_specialSnake == true) {
			m_timesSpecial++;
			if (m_timesSpecial == 10) {
				m_timesSpecial = 0;
				m_specialSnake = false;
			}
		}

		checkCollision();

		if (m_fruit == m_snake.getLimbs()[0])
			eatFruit();
		if (m_specialFruit == m_snake.getLimbs()[0])
			eatSpecial();

		if (spawnPossible())
			createTempWall();

		updateTempWalls();
		updateSnake();
		spawnSpecialFruit();
		updateWalls(); //TODO: find a way to not call this each time
	}

	template<size_t size>
	inline void Matrix<size>::moveSnakeUp() {
		//Removing the last tail part
		setCellType(m_snake.getLimbs().back(), '0');
		m_snake.moveUp();
		updateBoard();
	}

	template<size_t size>
	inline void Matrix<size>::moveSnakeDown() {
		setCellType(m_snake.getLimbs().back(), '0');
		m_snake.moveDown();
		updateBoard();
	}

	template<size_t size>
	inline void Matrix<size>::moveSnakeRight() {
		setCellType(m_snake.getLimbs().back(), '0');
		m_snake.moveRight();
		updateBoard();
	}

	template<size_t size>
	inline void Matrix<size>::moveSnakeLeft() {
		setCellType(m_snake.getLimbs().back(), '0');
		m_snake.moveLeft();
		updateBoard();
	}

	template<size_t size>
	inline void Matrix<size>::activateSpecialFruit() {
		m_specialSnake = true;
		m_timesSpecial = 0;
		m_specialFruits--;
	}

	template<size_t size>
	void Matrix<size>::createWalls() {
		//Gathering coords of each direction 
		// around the edges
		size_t max = size - 1;
		for (int i = 0; i < max; i++) {
			m_walls.emplace_back(i, 0);
			m_walls.emplace_back(max, i);
			m_walls.emplace_back(max - i, max);
			m_walls.emplace_back(0, max - i);
		}

		/*for (auto wall : walls) {
			setCell(wall, 'w');
		}*/
	}

	template<size_t size>
	void Matrix<size>::createTempWall() {

		Coords wall(0, 0);
		do {
			newRdmCoords(wall);
			//Cannot spawn near snake's head
		} while (isNearSnakeHead(wall.getX(), wall.getY()));
		
		m_tempWalls.push_back(wall);
	}

	template<size_t size>
	inline void Matrix<size>::spawnSpecialFruit() {

		if (spawnPossible() && m_specialFruitOnTable == false) {
			newRdmCoords(m_specialFruit);
			m_specialFruitOnTable = true;
			setCellType(m_specialFruit, 'b');
		}
	}

	template<size_t size>
	void Matrix<size>::newRdmCoords(Coords& c) {
		//Cannot spawn on walls
		size_t start = 1;
		size_t range = size - 2;
		unsigned rdmX;
		unsigned rdmY;

		do {
			rdmX = rand() % range + start;
			rdmY = rand() % range + start;

		} while (m_matrix[rdmY][rdmX].getType() != '0');

		c.setCoords(rdmX, rdmY);
	}

	template<size_t size>
	bool Matrix<size>::spawnPossible() {
		unsigned possibility = rand() % 25;
		return possibility == 1;
	}

	template<size_t size>
	bool Matrix<size>::isNearSnakeHead(unsigned x, unsigned y) {
		unsigned snakeX = m_snake.getX();
		unsigned snakeY = m_snake.getY();

		return ((x == snakeX + 1) || (x == snakeX - 1) ||
				(y == snakeY + 1) || (y == snakeY - 1) );
	}

	template<size_t size>
	inline void Matrix<size>::breakTempWall(unsigned x, unsigned y) {

		for (auto it = m_tempWalls.begin(); it != m_tempWalls.end(); ++it) {
			if (it->getX() == x && it->getY() == y) {
				m_tempWalls.erase(it);
				break;
			}
		}
	}

	template<size_t size>
	void Matrix<size>::emptyBoard() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				m_matrix[i][j].setType('0');
			}
		}
	}

	template<size_t size>
	inline void Matrix<size>::updateWalls() {
		for (auto wall : m_walls) {
			setCellType(wall, 'w');
		}
	}

	template<size_t size>
	inline void Matrix<size>::updateTempWalls() {
		for (auto wall : m_tempWalls) {
			setCellType(wall, 't');
		}
	}

	template<size_t size>
	void Matrix<size>::updateSnake() {
		auto limbs = m_snake.getLimbs();
		unsigned length = limbs.size();

		//Creating the blinking effect
		static bool specialColor = false;
		if (m_specialSnake) {
			specialColor = !specialColor;
		} else {
			specialColor = false;
		}

		//Updating head
		if (specialColor) {
			setCellType(limbs.front(), 'v');
		} else {
			setCellType(limbs.front(), 'h');
		}

		//Ignoring head, updating rest
		for (uint8_t i = 1; i < length; i++) {
			if (specialColor) {
				setCellType(limbs[i], 'i');
			} else {
				setCellType(limbs[i], 's');
			}
		}
	}

	template<size_t size>
	inline void Matrix<size>::setCellType(Coords c, char type) {
		m_matrix[c.getY()][c.getX()].setType(type);
	}

	template<size_t size>
	inline char Matrix<size>::getCellType(Coords c) {
		return m_matrix[c.getY()][c.getX()].getType();
	}

	template<size_t size>
	inline void Matrix<size>::checkCollision() {

		char cellType = getCellType(m_snake.getLimbs().front());

		if (!m_specialSnake) {
			if (cellType == 'w' || cellType == 's' || cellType == 't') {
				std::cout << "Snake Crashed. Game Over!" << std::endl;
				m_gameOver = true;
			}
		} else {
			if (cellType == 't') {
				//If special, can break walls
				breakTempWall(m_snake.getX(), m_snake.getY());
			}
		}
	}

	template<size_t size>
	inline void Matrix<size>::eatFruit() {
		m_snake.prepareNewLimb();

		setCellType(m_fruit, '0');
		newRdmCoords(m_fruit);
		setCellType(m_fruit, 'f');
	}

	template<size_t size>
	inline void Matrix<size>::eatSpecial() {
		//Maximum of 8 special fruits gathered
		if (m_specialFruits <= 8)
			m_specialFruits++;

		m_specialFruitOnTable = false;
		setCellType(m_specialFruit, '0');
	}
}
#endif