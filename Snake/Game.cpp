#include "Game.h"

Game::Game(size_t size) : 
	running(true), 
	speed(0.3),
	matrix(), 
	windowSize(size),
	inputManager(inputManager.getInstance()), 
	graphics("The amazing Snake!", size, 150) {

	fillTextureBank();
}

void Game::menu() {
	bool menu = true;

	std::cout << "Welcome to the Snake Game\n"
		"[1] - Start Game\n"
		"[2] - Leaderboard\n"
		"[ESC] - Exit Game" << std::endl;

	graphics.clearScreen();
	graphics.readyTexture('g', 0, 0, 100, 100);
	graphics.readyTexture('t', 10, 20, 80, 20);
	graphics.readyTexture('t', 10, 50, 80, 20);
	graphics.readyTexture('p', 15, 25, 40, 10);
	graphics.readyTexture('l', 15, 55, 40, 10);
	graphics.renderGraphics();
	while (menu) {
		
		inputManager.update();
		
		/* Escapes */
		if (inputManager.keyDown(SDL_SCANCODE_ESCAPE)) {
			menu = false;
		}
		if (SDL_HasEvent(SDL_QUIT)) {
			menu = false;
		}

		/* Actions */
		if (inputManager.keyDown(SDL_SCANCODE_1)) {
			renderGameMargin();
			start();
			menu = false;
		}
	}
}

void Game::start() {
	std::cout << matrix;

	Direction newDir = Direction::up;
	Direction direction = newDir;

	//Have taken away renderclear. This might cause issues? <<<----
	
	auto timeCount = time.now();

	renderMatrix();

	while (running) {
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
		if (inputManager.keyDown(SDL_SCANCODE_UP)) {
			newDir = Direction::up;

		} else if (inputManager.keyDown(SDL_SCANCODE_DOWN)) {
			newDir = Direction::down;

		} else if (inputManager.keyDown(SDL_SCANCODE_RIGHT)) {
			newDir = Direction::right;

		} else if (inputManager.keyDown(SDL_SCANCODE_LEFT)) {
			newDir = Direction::left;

		}

		//Will only move once every {speed} seconds
		std::chrono::duration<double> diff = time.now() - timeCount;
		if (diff.count() > speed) {
			timeCount = time.now();

			//The snake cannot move in the direction opposite of the current
			if (!(newDir == !direction)) {
				direction = newDir;
			}
			

			if (direction == Direction::up) {
				
				std::cout << "UP" << std::endl;
				matrix.moveUp();

			} else if (direction == Direction::down) {
				
				std::cout << "DOWN" << std::endl;
				matrix.moveDown();

			} else if (direction == Direction::right) {
				
				std::cout << "RIGHT" << std::endl;
				matrix.moveRight();

			} else if (direction == Direction::left) {
				
				std::cout << "LEFT" << std::endl;
				matrix.moveLeft();
			}
			std::cout << matrix;
			renderMatrix();
		}
		if (matrix.isGameOver()) {
			running = false;
		}
	}
	if(matrix.isGameOver())
		gameOver();
}

void Game::gameOver() {
	graphics.readyTexture('o', 0, 0, 100, 100);
	graphics.renderGraphics();
	while (!(inputManager.keyDown(SDL_SCANCODE_RETURN) || inputManager.keyDown(SDL_SCANCODE_ESCAPE))) {
		inputManager.update();
	}
}

void Game::renderGameMargin() {
	graphics.clearScreen();
	graphics.readyTexture('g', 0, 0, 100, 100);
	graphics.renderGraphics();
}

void Game::renderMatrix() {
	auto matrixView = matrix.getLayout();
	size_t size = matrixView.size();
	bool specialSnake = matrix.isSpecialSnake();
	static bool specialColor = false;

	if (specialSnake)
		specialColor = !specialColor;

	for (Uint8 i = 0; i < size; i++) {
		for (Uint8 j = 0; j < size; j++) {
			auto cell = matrix.getLayout()[i][j];
			char type = cell.getType();

			//Will only update texture, if content has changed <<<< some problems i must admit
			//if ( type != cell.getPrevType()) {

			if (specialSnake && specialColor) {
				if (type == 'h') {
					type = 'v';
				} else if (type == 's') {
					type = 'i';
				}
			}
			graphics.readyTexture(type, j, i, size);
			//}
		}
	}
}

void Game::fillTextureBank() {

	//should have some way of indexing these with titles or something
	graphics.createTexture("Sprites/Black.bmp", '0');
	graphics.createTexture("Sprites/BrownBlock.bmp", 'w');
	graphics.createTexture("Sprites/SnakeHead.bmp", 'h');
	graphics.createTexture("Sprites/SnakeBlock.bmp", 's');
	graphics.createTexture("Sprites/InvertedSnake.bmp", 'i');
	graphics.createTexture("Sprites/InvertedHead.bmp", 'v');
	graphics.createTexture("Sprites/Fruit.bmp", 'f');
	graphics.createTexture("Sprites/SpecialFruit.bmp", 'e');
	graphics.createTexture("Sprites/GameOver.bmp", 'o');
	graphics.createTexture("Sprites/Green.bmp", 'g');
	graphics.createTexture("Sprites/TextBox.bmp", 't');
	graphics.createTexture("Sprites/StartGame.bmp", 'p');
	graphics.createTexture("Sprites/Leaderboard.bmp", 'l');
}
