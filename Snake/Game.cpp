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

	/*std::cout << "Welcome to the Snake Game\n"
		"[1] - Start Game\n"
		"[2] - Leaderboard\n"
		"[ESC] - Exit Game" << std::endl;*/

	graphics.clearScreen();
	graphics.readyTexture('g', 0, 0, 100, 100);
	graphics.readyTexture('t', 10, 20, 80, 20);
	graphics.readyTexture('t', 10, 50, 80, 20);
	graphics.readyText('s', 15, 25);
	graphics.readyText('l', 15, 55);
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
	//std::cout << matrix;

	Direction newDir = Direction::up;
	Direction direction = newDir;

	//Have taken away renderclear. This might cause issues? <<<----
	
	auto timeCount = time.now();

	updateMatrix();

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

		/* Actions */
		if (inputManager.keyDown(SDL_SCANCODE_X) && matrix.getSpecialFruits() > 0) {
			matrix.activateSpecialFruit();
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
				
				//std::cout << "UP" << std::endl;
				matrix.moveUp();

			} else if (direction == Direction::down) {
				
				//std::cout << "DOWN" << std::endl;
				matrix.moveDown();

			} else if (direction == Direction::right) {
				
				//std::cout << "RIGHT" << std::endl;
				matrix.moveRight();

			} else if (direction == Direction::left) {
				
				//std::cout << "LEFT" << std::endl;
				matrix.moveLeft();
			}
			std::cout << matrix;
			updateMatrix();
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
	graphics.readyText('c', 50, 5);
	graphics.renderGraphics();
}

inline unsigned Game::extractDigit(unsigned number, unsigned exponent) {
	return static_cast<unsigned>(number / pow(10, exponent)) % 10;
}

void Game::clearStats() {
	size_t size = matrix.getSize();
	graphics.readyTexture('g', 70, 5, 15, 10);
	for (int i = 0; i < size; i++) {
		graphics.readyTexture('g', i, size + 1, size);
	}
}

void Game::updateMatrix() {
	static bool specialColor = false;
	auto matrixView = matrix.getLayout();
	size_t size = matrix.getSize();
	bool specialSnake = matrix.isSpecialSnake();
	unsigned specialFruits = matrix.getSpecialFruits();
	unsigned snakeSize = matrix.getSnakeSize();

	if (specialSnake)
		specialColor = !specialColor;

	for (Uint8 i = 0; i < size; i++) {
		for (Uint8 j = 0; j < size; j++) {
			auto cell = matrix.getLayout()[i][j];
			char type = cell.getType();

			//TWO BUGS:
			//When eating first fruit: Matrix shows: tail only visible, not head for one 'frame'
			//When eating second fruit: Matrix shows correct, but renders two eye-blocks after eachother

			//Will only update texture, if content has changed <<<< some problems i must admit... Weird bug snake gets double eyes everytime going from 2 to 3 eyes
			if (cell.hasChanged()) {

				if (specialSnake && specialColor) {
					if (type == 'h') {
						type = 'v';
					} else if (type == 's') {
						type = 'i';
					}
				}
				graphics.readyTexture(type, j, i, size);
			}
		}
	}

	clearStats();
	//Setting stats
	for (int i = 0; i < specialFruits; i++) {
		graphics.readyTexture('b', i, size + 1, size);
	}

	graphics.readyText('0' + extractDigit(snakeSize - 1, 2), 70, 5);
	graphics.readyText('0' + extractDigit(snakeSize - 1, 1), 75, 5);
	graphics.readyText('0' + extractDigit(snakeSize - 1, 0), 80, 5);
}

void Game::fillTextureBank() {
	//Textures
	graphics.createTexture("Sprites/Black.bmp", '0');
	graphics.createTexture("Sprites/BrownBlock.bmp", 'w');
	graphics.createTexture("Sprites/SnakeHead.bmp", 'h');
	graphics.createTexture("Sprites/SnakeBlock.bmp", 's');
	graphics.createTexture("Sprites/InvertedSnake.bmp", 'i');
	graphics.createTexture("Sprites/InvertedHead.bmp", 'v');
	graphics.createTexture("Sprites/Fruit.bmp", 'f');
	graphics.createTexture("Sprites/SpecialFruit.bmp", 'b');
	graphics.createTexture("Sprites/GameOver.bmp", 'o');
	graphics.createTexture("Sprites/Green.bmp", 'g');
	graphics.createTexture("Sprites/TextBox.bmp", 't');

	//Texts
	graphics.createText("Start Game", 85, 's');
	graphics.createText("Leaderboard", 85, 'l');
	graphics.createText("Score: ", 60, 'c');

	//Numbers
	graphics.createText("0", 60, '0');
	graphics.createText("1", 60, '1');
	graphics.createText("2", 60, '2');
	graphics.createText("3", 60, '3');
	graphics.createText("4", 60, '4');
	graphics.createText("5", 60, '5');
	graphics.createText("6", 60, '6');
	graphics.createText("7", 60, '7');
	graphics.createText("8", 60, '8');
	graphics.createText("9", 60, '9');
}
