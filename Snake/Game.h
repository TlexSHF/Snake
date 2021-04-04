#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <chrono>
#include <string>
#include "InputManager.h"
#include "Matrix.h"
#include "GraphicsManager.h"
#include "Direction.h"
#include "Coords.h"

class Game {
public:
	Game(size_t size);
	void menu();
private:
	InputManager& inputManager;
	Matrix<10> matrix;
	GraphicsManager graphics;
	std::chrono::high_resolution_clock time;
	size_t windowSize;

	bool running;
	double speed;

	unsigned score;

	void start();
	void gameOver();

	void renderGameMargin();
	unsigned extractDigit(unsigned number, unsigned exponent);
	void clearStats();
	void updateMatrix();
	void fillTextureBank();

	bool mouseInsideArea(SDL_Rect rectangle);
};

#endif