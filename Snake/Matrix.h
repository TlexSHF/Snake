#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <array>
#include <vector>
#include "Cell.h"
#include "Coords.h"
#include "Snake.h"

template<const size_t size>
class Matrix {
public:
	Matrix();

	/* Getters */
	std::array<std::array<Cell, size>, size> getLayout();
	bool isGameOver();
	bool isSpecialSnake();
	unsigned getSpecialFruits();
	size_t getSize();
	size_t getSnakeSize();

	/* Updating */
	void updateBoard();
	void checkCollision();

	/* Movement */
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();

	/* Actions */
	void activateSpecialFruit();

	friend std::ostream& operator<<(std::ostream& os, Matrix<size>& m) {
		for (auto column : m.matrix) {
			for (auto cell : column) {
				if (cell.getType() == '0') {
					os << "  ";
				} else {
					os << cell << " ";
				}
			}
			os << std::endl;
		}
		os << "\n SpecialFruits gathered: " << m.specialFruits << std::endl;;
		return os;
	}

private:
	std::array<std::array<Cell, size>, size> matrix;
	std::vector<Coords> walls;
	std::vector<Coords> tempWalls;
	Snake snake;
	Coords fruit;
	Coords specialFruit;
	unsigned specialFruits = 0;
	bool specialSnake = false;
	bool visibleSpecial = false;
	bool gameOver = false;
	unsigned timesSpecial = 0;

	/* Creations */
	void createWalls();
	void createTempWall();
	void newRdmCoords(Coords & c);
	bool possibleSpawn();

	void breakTempWall(unsigned x, unsigned y);

	/* Updating objects */
	void emptyBoard();
	void updateWalls();
	void updateTempWalls();
	void updateSnake();
	void updateFruit();

	void setCellType(Coords c, char type);
	char getCellType(Coords c);

	/* Actions */
	void eatFruit();
	void eatSpecial();
};

template<size_t size>
inline Matrix<size>::Matrix() :
	matrix(),
	snake(4, 4, size), //THESE CAN NOT BE HARDCODED
	fruit(0, 0),
	specialFruit(0, 0) {

	srand(time(NULL)); //init psuedo random number

	emptyBoard();
	createWalls();

	newRdmCoords(snake.getLimbs()[0]); //TODO This doesn't do much it seems.
	newRdmCoords(fruit);
	setCellType(fruit, 'f');

	updateBoard();
}

template<size_t size>
inline std::array<std::array<Cell, size>, size> Matrix<size>::getLayout() {
	return matrix;
}

template<size_t size>
inline bool Matrix<size>::isGameOver() {
	return gameOver;
}

template<size_t size>
inline bool Matrix<size>::isSpecialSnake() {
	return specialSnake;
}

template<size_t size>
inline unsigned Matrix<size>::getSpecialFruits() {
	return specialFruits;
}

template<size_t size>
inline size_t Matrix<size>::getSize() {
	return size;
}

template<size_t size>
inline size_t Matrix<size>::getSnakeSize() {
	return snake.getSize();
}

template<size_t size>
void Matrix<size>::updateBoard() {
	//placing possible new limb
	if (snake.hasNewLimb()) {
		snake.addLimb();
	}

	//Snake can only be special for a couple of moves
	//If special snake, it will not collide
	if (specialSnake == true) {
		timesSpecial++;
		if (timesSpecial == 10) {
			timesSpecial = 0;
			specialSnake = false;
		}
	}
	
	checkCollision();

	if (fruit == snake.getLimbs()[0])
		eatFruit();
	if (specialFruit == snake.getLimbs()[0])
		eatSpecial();

	if(possibleSpawn())
		createTempWall();

	updateTempWalls();
	updateSnake();
	updateFruit();
	updateWalls(); //TODO: find a way to not call this each time
}

template<size_t size>
inline void Matrix<size>::checkCollision() {

	char cellType = getCellType(snake.getLimbs().front());

	if (!specialSnake) {
		if (cellType == 'w' || cellType == 's' || cellType == 't') {
			std::cout << "Snake Crashed. Game Over!" << std::endl;
			gameOver = true;
		}
	} else {
		if (cellType == 't') {
			//If special, can break walls
			breakTempWall(snake.getX(), snake.getY());
		}
	}
}

template<size_t size>
inline void Matrix<size>::moveUp() {
	//Removing the last tail part
	setCellType(snake.getLimbs().back(), '0');
	snake.moveUp();
	updateBoard();
}

template<size_t size>
inline void Matrix<size>::moveDown() {
	setCellType(snake.getLimbs().back(), '0');
	snake.moveDown();
	updateBoard();
}

template<size_t size>
inline void Matrix<size>::moveRight() {
	setCellType(snake.getLimbs().back(), '0');
	snake.moveRight();
	updateBoard();
}

template<size_t size>
inline void Matrix<size>::moveLeft() {
	setCellType(snake.getLimbs().back(), '0');
	snake.moveLeft();
	updateBoard();
}

template<size_t size>
inline void Matrix<size>::activateSpecialFruit() {
	specialSnake = true;
	timesSpecial = 0;
	specialFruits--;
}

template<size_t size>
void Matrix<size>::createWalls() {
	//Gathering coords of each direction 
	// around the edges
	size_t max = size - 1;
	for (int i = 0; i < max; i++) {
		walls.emplace_back(i, 0);
		walls.emplace_back(max, i);
		walls.emplace_back(max - i, max);
		walls.emplace_back(0, max - i);
	}

	/*for (auto wall : walls) {
		setCell(wall, 'w');
	}*/
}

template<size_t size>
void Matrix<size>::createTempWall() {
	
	Coords start(0, 0);
	newRdmCoords(start);
	tempWalls.push_back(start);
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

	} while (matrix[rdmY][rdmX].getType() != '0');

	c.setCoords(rdmX, rdmY);
}

template<size_t size>
bool Matrix<size>::possibleSpawn() {
	//sannsynlighet for å spawne en special fruit
	unsigned possibility = rand() % 25;
	return possibility == 1;
}

template<size_t size>
inline void Matrix<size>::breakTempWall(unsigned x, unsigned y) {

	for (auto it = tempWalls.begin(); it != tempWalls.end(); ++it) {
		if (it->getX() == x && it->getY() == y) {
			tempWalls.erase(it);
			break;
		}
	}
}

template<size_t size>
void Matrix<size>::emptyBoard() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrix[i][j].setType('0');
		}
	}
}

template<size_t size>
inline void Matrix<size>::updateWalls() {
	for (auto wall : walls) {
		setCellType(wall, 'w');
	}
}

template<size_t size>
inline void Matrix<size>::updateTempWalls() {
	for (auto wall : tempWalls) {
		setCellType(wall, 't');
	}
}

template<size_t size>
void Matrix<size>::updateSnake() {
	auto limbs = snake.getLimbs();

	//updating head
	//if (getCell(limbs.front()) != 'w')
		setCellType(limbs.front(), 'h');

	//updating first tail
	if(limbs.size() > 1 /*&& getCell(limbs[1]) != 'w'*/)
		setCellType(limbs[1], 's');
}

template<size_t size>
inline void Matrix<size>::updateFruit() {
	//matrix[fruit.getY()][fruit.getX()].setType('f');

	if (possibleSpawn() && visibleSpecial == false) {
		newRdmCoords(specialFruit);
		visibleSpecial = true;
		setCellType(specialFruit, 'b');
	}
}

template<size_t size>
inline void Matrix<size>::setCellType(Coords c, char type) {
	matrix[c.getY()][c.getX()].setType(type);
}

template<size_t size>
inline char Matrix<size>::getCellType(Coords c) {
	return matrix[c.getY()][c.getX()].getType();
}

template<size_t size>
inline void Matrix<size>::eatFruit() {
	snake.prepareNewLimb();
	 
	setCellType(fruit, '0');
	newRdmCoords(fruit);
	setCellType(fruit, 'f');
}

template<size_t size>
inline void Matrix<size>::eatSpecial() {
	//Kan maksimalt samle på 8 speical fruits
	if(specialFruits <= 8)
		specialFruits++;
	
	visibleSpecial = false;
	setCellType(specialFruit, '0');
}

#endif