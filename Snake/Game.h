#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "InputManager.h"
#include "Matrix.h"
#include "GraphicsManager.h"

class Game {
public:
	Game();
	void start();
private:
	InputManager& inputManager;
	Matrix<10> matrix;
	GraphicsManager graphics;

	bool running;

	void renderMatrix();
};

#endif