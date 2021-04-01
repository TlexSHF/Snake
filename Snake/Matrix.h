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
	void createSnake();
	void newFruit(Coords& fruit);
	bool canSpawnSpecial();

	/* Updating objects */
	void emptyBoard();
	void updateWalls();
	void updateSnake();
	void updateFruit();

	/* Actions */
	void eatFruit();
	void eatSpecial();
};

template<size_t size>
inline Matrix<size>::Matrix() :
	matrix(),
	snake(4, 6, size), //THESE CAN NOT BE HARDCODED
	fruit(5, 6),
	specialFruit(7, 8) { //THESE CAN NOT BE HARDCODED

	srand(time(NULL)); //init psuedo random number

	createWalls();
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
void Matrix<size>::updateBoard() {

	//Snake can only be special for a couple of moves
	//If special snake, it will not collide
	if (specialSnake == true) {
		timesSpecial++;
		if (timesSpecial == 10) {
			timesSpecial = 0;
			specialSnake = false;
		}
	} else {
		checkCollision();
	}

	if (fruit == snake.getLimbs()[0])
		eatFruit();
	if (specialFruit == snake.getLimbs()[0])
		eatSpecial();

	emptyBoard();
	updateSnake();
	updateFruit();
	updateWalls();
}

template<size_t size>
inline void Matrix<size>::checkCollision() {

	unsigned y = snake.getY();
	unsigned x = snake.getX();
	
	if (matrix[y][x].getType() == 'w' || matrix[y][x].getType() == 's') {
		std::cout << "Snake Crashed. Game Over!" << std::endl;
		gameOver = true;
	}
}

template<size_t size>
inline void Matrix<size>::moveUp() {
	snake.moveUp();
	updateBoard();
}

template<size_t size>
inline void Matrix<size>::moveDown() {
	snake.moveDown();
	updateBoard();
}

template<size_t size>
inline void Matrix<size>::moveRight() {
	snake.moveRight();
	updateBoard();
}

template<size_t size>
inline void Matrix<size>::moveLeft() {
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
	for (int i = 0; i < size; i++) { //TODO: This has double wall on top right, left and bottom left
		walls.emplace_back(i, 0);
		walls.emplace_back(i, size - 1);
		walls.emplace_back(0, i);
		walls.emplace_back(size - 1, i);
	}
}

template<size_t size>
inline void Matrix<size>::createSnake() {
	matrix[snake.getY()][snake.getX()].setType('s');
}

template<size_t size>
inline void Matrix<size>::newFruit(Coords& f) {
	//Cannot spawn on walls
	size_t start = 1;
	size_t range = size - 2;
	unsigned rdmX;
	unsigned rdmY;

	do {
		//Should set seed?
		rdmX = rand() % range + start;
		rdmY = rand() % range + start;

	} while (matrix[rdmY][rdmX].getType() != '0');

	f.setCoords(rdmX, rdmY);
}

template<size_t size>
inline bool Matrix<size>::canSpawnSpecial() {
	//sannsynlighet for å spawne en special fruit
	unsigned possibility = rand() % 25;
	if (possibility == 1) {
		visibleSpecial = true;
		return true;
	} else {
		return false;
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
void Matrix<size>::updateWalls() {
	for (auto wall : walls) {
		matrix[wall.getY()][wall.getX()].setType('w');
	}
}

template<size_t size>
void Matrix<size>::updateSnake() {
	for (auto limb : snake.getLimbs()) {
		matrix[limb.getY()][limb.getX()].setType('s');
	}
	matrix[snake.getY()][snake.getX()].setType('h'); //Very sloppy code. Writes over once
}

template<size_t size>
inline void Matrix<size>::updateFruit() {
	matrix[fruit.getY()][fruit.getX()].setType('f');

	if(canSpawnSpecial() || visibleSpecial)
		matrix[specialFruit.getY()][specialFruit.getX()].setType('b');
}

template<size_t size>
inline void Matrix<size>::eatFruit() {
	snake.addLimb();
	newFruit(fruit);
}

template<size_t size>
inline void Matrix<size>::eatSpecial() {
	//Kan maksimalt samle på 8 speical fruits
	if(specialFruits <= 8)
		specialFruits++;
	
	visibleSpecial = false;
	newFruit(specialFruit);
}

#endif