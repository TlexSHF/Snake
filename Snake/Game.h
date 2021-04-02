#ifndef GAME_H
#define GAME_H

//#include <SDL_ttf.h> Litt mye styr for å legge til en font men kanskje jeg gjør det senere
#include <iostream>
#include <chrono>
#include "InputManager.h"
#include "Matrix.h"
#include "GraphicsManager.h"
#include "Direction.h"

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

	void start();
	void gameOver();

	void renderGameMargin();
	unsigned extractDigit(unsigned number, unsigned exponent);
	void clearStats();
	void updateMatrix();
	void fillTextureBank();
};

#endif