#include "Game.h"

Game::Game() : running(true), matrix(), inputManager(inputManager.getInstance()/*, graphics()*/){}

void Game::start() {
	std::cout << matrix;

	unsigned snakeX = 200;
	unsigned snakeY = 200;

	Direction direction = Direction::up;

	//Have taken away renderclear. This might cause issues? <<<----
	
	auto timeCount = time.now();

	renderMatrix();

	while (running) {
		//SDL_Rect coords = graphics.getCoords();
		
		//graphics.renderTexture(4, snakeX, snakeY);
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
			direction = Direction::up;

		} else if (inputManager.keyDown(SDL_SCANCODE_DOWN) /*|| inputManager.keyStillDown(SDL_SCANCODE_DOWN)*/) {
			direction = Direction::down;

		} else if (inputManager.keyDown(SDL_SCANCODE_RIGHT) /*|| inputManager.keyStillDown(SDL_SCANCODE_RIGHT)*/) {
			direction = Direction::right;

		} else if (inputManager.keyDown(SDL_SCANCODE_LEFT) /*|| inputManager.keyStillDown(SDL_SCANCODE_LEFT)*/) {
			direction = Direction::left;

		}

		std::chrono::duration<double> diff = time.now() - timeCount;
		if (diff.count() > 0.2) {
			timeCount = time.now();

			if (direction == Direction::up) {
				std::cout << "UP" << std::endl;
				matrix.moveUp();
				std::cout << matrix;
				renderMatrix();

			} else if (direction == Direction::down) {
				std::cout << "DOWN" << std::endl;
				matrix.moveDown();
				std::cout << matrix;
				renderMatrix();

			} else if (direction == Direction::right) {
				std::cout << "RIGHT" << std::endl;
				matrix.moveRight();
				std::cout << matrix;
				renderMatrix();

			} else if (direction == Direction::left) {
				std::cout << "LEFT" << std::endl;
				matrix.moveLeft();
				std::cout << matrix;
				renderMatrix();
			}
		}
	}
}

void Game::renderMatrix() {
	auto matrixView = matrix.getLayout();
	size_t size = matrixView.size();

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			auto cell = matrix.getLayout()[i][j];
			unsigned index = 0;

			if (matrix.isGameOver()) {
				graphics.renderTexture(5, j * 100, i * 100);
			} else {
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
	}
}
