#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>
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
		InputManager& m_inputManager;
		std::unique_ptr<Matrix<matrixSize>> m_matrix;
		GraphicsManager m_graphics;

		std::chrono::high_resolution_clock m_time;
		size_t m_windowSize;

		std::string m_playerName;
		bool m_running = true;
		double m_speed = 0;

		void loadingScreen();
		void startGame();
		void gameOver();
		
		void recieveUserInput(Direction& newDir);
		void moveSnake(Direction& direction);

		void renderGameMargin();
		void clearStats();
		void updateGraphics();

		std::string getImageFromType(char type);

		unsigned extractDigit(unsigned number, unsigned exponent);
		bool mouseInsideArea(SDL_Rect rectangle);

		void leaderBoard();
		void addNewScore(unsigned score);
		void updatePlayerName();

		void getFromLeaderBoard(std::vector<Score>& scores);
		void writeToLeaderBoard(const std::vector<Score>& scores);

		void waitForInput();
	};
}

#endif