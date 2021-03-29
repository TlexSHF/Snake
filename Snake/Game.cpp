#include "Game.h"

Game::Game() : running(true), matrix(), inputManager(inputManager.getInstance()/*, graphics()*/){}

void Game::start() {
	std::cout << matrix;

	while (running) {
		SDL_Renderer* renderer = graphics.getRenderer();
		SDL_Texture* texture = graphics.getTexture();
		SDL_Rect coords = graphics.getCoords();

		SDL_RenderCopy(
			renderer,
			texture,
			nullptr,
			&coords
		);

		inputManager.update();
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);


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
