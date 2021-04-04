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
	SDL_Rect txtBox1 = graphics.readyTexture('x', 10, 20, 80, 20);
	SDL_Rect txtBox2 = graphics.readyTexture('x', 10, 50, 80, 20);
	graphics.writeText("Start Game", 15, 25, 85);
	graphics.writeText("Leaderboard", 15, 55, 85);
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

		if (inputManager.mouseDown(SDL_BUTTON_LEFT)) {

			if (mouseInsideArea(txtBox1)) {
				renderGameMargin();
				start();
				menu = false;
			}

			if (mouseInsideArea(txtBox2)) {
				std::cout << "INSIDE 2" << std::endl;
			}
		}
	}
}

void Game::start() {
	//std::cout << matrix;

	Direction newDir = Direction::up;
	Direction direction = newDir;
	
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
	graphics.writeText("Score: ", 50, 5, 60);
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

	score = matrix.getSnakeSize() - 1;
	//Every 5 score speed gets quicker
	if (score != 0 && score % 5 == 0) {
		speed -= 0.002;
	}

	if (specialSnake)
		specialColor = !specialColor;

	for (Uint8 i = 0; i < size; i++) {
		for (Uint8 j = 0; j < size; j++) {
			auto cell = matrix.getLayout()[i][j];
			char type = cell.getType();

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

	graphics.writeText(std::to_string(extractDigit(score, 2)), 70, 5, 60);
	graphics.writeText(std::to_string(extractDigit(score, 1)), 75, 5, 60);
	graphics.writeText(std::to_string(extractDigit(score, 0)), 80, 5, 60);
}

void Game::fillTextureBank() {
	//Textures
	graphics.createTexture("Sprites/GameOver.bmp", 'o');
	graphics.createTexture("Sprites/Green.bmp", 'g');
	graphics.createTexture("Sprites/TextBox.bmp", 'x');

	//Cells
	graphics.createTexture("Sprites/Black.bmp", '0');			//Background
	graphics.createTexture("Sprites/BrownBlock.bmp", 'w');		//Wall
	graphics.createTexture("Sprites/BrownBlock.bmp", 't');		//Temp Wall
	graphics.createTexture("Sprites/SnakeHead.bmp", 'h');		//Head
	graphics.createTexture("Sprites/SnakeBlock.bmp", 's');		//Tail
	graphics.createTexture("Sprites/InvertedHead.bmp", 'v');	//Inverted Head
	graphics.createTexture("Sprites/InvertedSnake.bmp", 'i');	//Inverted Tail
	graphics.createTexture("Sprites/Fruit.bmp", 'f');			//Red Fruit
	graphics.createTexture("Sprites/SpecialFruit.bmp", 'b');	//Blue Special Fruit
}

bool Game::mouseInsideArea(SDL_Rect rectangle) {
	int x = inputManager.getMouseX();
	int y = inputManager.getMouseY();
	
	return (
		x >= rectangle.x && 
		x <= rectangle.x + rectangle.w &&
		y >= rectangle.y && 
		y <= rectangle.y + rectangle.h
	);
}
