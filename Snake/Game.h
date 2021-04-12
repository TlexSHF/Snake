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

		bool m_running = true;
		double m_speed = 0;

		//std::mutex m_graphicsLock; //Not sure if we needs these
		//std::condition_variable m_graphicsCV;

		void countDown();
		void startGame();
		void gameOver();
		
		void recieveUserInput(Direction& newDir);
		void moveSnake(Direction& direction);
		void renderGraphics();

		void renderGameMargin();
		void clearStats();
		void updateGraphics();
		void fillTextureBank();

		std::string getImageFromType(char type);

		unsigned extractDigit(unsigned number, unsigned exponent);
		bool mouseInsideArea(SDL_Rect rectangle); //TODO MOVED THIS DOWN ^

		void leaderBoard();
		void addNewScore(unsigned score);

		void getFromLeaderBoard(std::vector<Score>& scores);
		void writeToLeaderBoard(const std::vector<Score>& scores);

		void waitForInput();
		void userTextInput();
	};
}

#endif