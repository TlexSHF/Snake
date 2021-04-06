#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <algorithm>
#include <vector>
#include "InputManager.h"
#include "Matrix.h"
#include "GraphicsManager.h"
#include "Direction.h"
#include "Coords.h"
#include "Score.h"

namespace snake {
	const size_t matrixSize = 10;

	class Game {
	public:
		Game(size_t windowSize);
		void menu();
	private:
		InputManager& inputManager;
		std::unique_ptr<Matrix<matrixSize>> matrix;
		GraphicsManager graphics;

		std::chrono::high_resolution_clock time;
		size_t windowSize;

		double speed = 0;
		unsigned score = 0;

		void start();
		void gameOver();

		void renderGameMargin();
		void clearStats();
		void updateMatrix();
		void fillTextureBank();

		unsigned extractDigit(unsigned number, unsigned exponent);
		bool mouseInsideArea(SDL_Rect rectangle); //TODO MOVED THIS DOWN ^

		void leaderBoard();
		void addNewScore(std::string name, unsigned score);
	};
}

#endif