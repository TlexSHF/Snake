#include "Game.h"

Game::Game() : running(true), matrix(), inputManager(inputManager.getInstance()/*, graphics()*/){}

void Game::start() {
	std::cout << matrix;

	unsigned snakeX = 200;
	unsigned snakeY = 200;

	while (running) {
		//SDL_Rect coords = graphics.getCoords();

		//This shall not happen every update, but this is a start. Should happen every press of button (changes the coordinates)
		auto matrixView = matrix.getLayout();
		size_t size = matrixView.size();
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				auto cell = matrix.getLayout()[i][j];
				unsigned index = 0;

				switch (cell.getType()) {
				case '0':
					index = 0;
					break;
				case 'w':
					index = 1;
					break;
				case 's':
					index = 2;
					break;
				case 'f':
					index = 3;
					break;
				case 'h':
					index = 4;
					break;
				}

				graphics.renderTexture(index, j * 100, i * 100);
			}
		}

		/*graphics.renderTexture(4, snakeX, snakeY);
		graphics.renderTexture(1, 400, 200);
		graphics.renderTexture(6, snakeX, snakeY);*/
		graphics.renderGraphics();
		inputManager.update();

		/* Escapes */
		if (inputManager.keyDown(SDL_SCANCODE_ESCAPE)) {
			running = false;
		}
		if (SDL_HasEvent(SDL_QUIT)) {
			running = false;
		}

		/* Arrows - Input */
		if (inputManager.keyDown(SDL_SCANCODE_UP) /*|| inputManager.keyStillDown(SDL_SCANCODE_UP)*/) {
			std::cout << "UP" << std::endl;
			matrix.moveUp();
			std::cout << matrix;

		} else if (inputManager.keyDown(SDL_SCANCODE_DOWN) /*|| inputManager.keyStillDown(SDL_SCANCODE_DOWN)*/) {
			std::cout << "DOWN" << std::endl;
			matrix.moveDown();
			std::cout << matrix;

		} else if (inputManager.keyDown(SDL_SCANCODE_RIGHT) /*|| inputManager.keyStillDown(SDL_SCANCODE_RIGHT)*/) {
			std::cout << "RIGHT" << std::endl;
			matrix.moveRight();
			std::cout << matrix;

		} else if (inputManager.keyDown(SDL_SCANCODE_LEFT) /*|| inputManager.keyStillDown(SDL_SCANCODE_LEFT)*/) {
			std::cout << "LEFT" << std::endl;
			matrix.moveLeft();
			std::cout << matrix;

		} else {
			//std::cout << "STOP" << std::endl;
		}
	}
}
