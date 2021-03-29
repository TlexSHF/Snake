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

	/* Updating */
	void updateBoard();
	void checkCollision();

	/* Movement */
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();

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
		return os;
	}

private:
	std::array<std::array<Cell, size>, size> matrix;
	std::vector<Coords> walls;
	Snake snake;
	Coords fruit;

	/* Creations */
	void createWalls();
	void createSnake();
	void newFruit();

	/* Updating objects */
	void emptyBoard();
	void updateWalls();
	void updateSnake();
	void updateFruit();

	/* Actions */
	void eatFruit();
};

template<size_t size>
inline Matrix<size>::Matrix() : matrix(), snake(4, 6), fruit(5, 6) { //THIS CAN NOT BE HARDCODED
	srand(time(NULL)); //init psuedo random number

	createWalls();
	updateBoard();
}

template<size_t size>
void Matrix<size>::updateBoard() {

	checkCollision();
	if (fruit == snake.getLimbs()[0])
		eatFruit();

	emptyBoard();
	updateSnake();
	updateFruit();
	updateWalls();
}

template<size_t size>
inline void Matrix<size>::checkCollision() {

	unsigned y = snake.getY();
	unsigned x = snake.getX();

	if (matrix[y][x].getType() == 'w' || matrix[y][x].getType() == 's')
		std::cout << "Snake Crashed. Game Over!" << std::endl;
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
inline void Matrix<size>::newFruit() {
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

	fruit.setCoords(rdmX, rdmY);
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
}

template<size_t size>
inline void Matrix<size>::eatFruit() {
	snake.addLimb();
	newFruit();
}

#endif