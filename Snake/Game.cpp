#include "Game.h"

namespace snake {

	Game::Game(size_t windowSize) :
		m_matrix(),
		m_windowSize(windowSize),
		m_inputManager(m_inputManager.getInstance()),
		m_graphics("The amazing Snake!", windowSize, 150) {

		fillTextureBank();
	}

	void Game::menu() {
		bool menu = true;

		while (menu) {
			m_graphics.clearScreen();
			m_graphics.drawTexture("Sprites/Green.bmp", 0, 0, 100, 100);
			SDL_Rect txtBox1 = m_graphics.drawTexture("Sprites/TextBox.bmp", 10, 20, 80, 20);
			SDL_Rect txtBox2 = m_graphics.drawTexture("Sprites/TextBox.bmp", 10, 50, 80, 20);
			m_graphics.writeText("Start Game", 15, 25, 85);
			m_graphics.writeText("Leaderboard", 15, 55, 85);
			m_graphics.renderGraphics();

			m_inputManager.update();

			/* Escapes */
			if (m_inputManager.keyDown(SDL_SCANCODE_ESCAPE)) {
				menu = false;
			}
			if (SDL_HasEvent(SDL_QUIT)) {
				menu = false;
			}

			/* Actions */

			if (m_inputManager.mouseDown(SDL_BUTTON_LEFT)) {

				if (mouseInsideArea(txtBox1)) {
					countDown();
					renderGameMargin();
					start();
				}

				if (mouseInsideArea(txtBox2)) {
					leaderBoard();
				}
			}
		}
	}

	void Game::countDown() {
		m_graphics.clearScreen();
		m_graphics.writeText("3", 40, 40, 100);
		m_graphics.renderGraphics();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_graphics.clearScreen();
		m_graphics.writeText("2", 40, 40, 100);
		m_graphics.renderGraphics();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_graphics.clearScreen();
		m_graphics.writeText("1", 40, 40, 100);
		m_graphics.renderGraphics();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	void Game::start() {
		m_running = true;
		m_speed = 0.3;

		Direction newDir = Direction::up;
		Direction direction = newDir;

		//std::thread outputThread([this] { updateGraphics(); });
		//std::thread outputThread(&updateGraphics);
		std::thread inputThread([this, &newDir] { recieveUserInput(newDir); });

		m_matrix = std::make_unique<Matrix<matrixSize>>();

		auto timeCount = m_time.now();

		updateGraphics(); //THIS DODODODODO
		//graphicsCV.notify_one();

		while (m_running) {

			//std::this_thread::sleep_for(std::chrono::milliseconds(1));

			m_graphics.renderGraphics();
			m_inputManager.update();

			//recieveUserInput(newDir);

			//std::cout << "outside\n";
			//Will only move once every {speed} seconds
			std::chrono::duration<double> diff = m_time.now() - timeCount;
			if (diff.count() > m_speed) {
				//std::cout << "tick\n";
				timeCount = m_time.now();

				//The snake cannot move in the direction opposite of the current
				if (!(newDir == !direction)) {
					direction = newDir;
				}

				moveSnake(direction);
				//std::thread gameThread([this, &direction] { moveSnake(direction); });
				//std::thread gameThread(&moveSnake, direction);
				//gameThread.join();
			}
			if (m_matrix->isGameOver()) {
				gameOver();
				m_running = false;
			}
			
			
		}
		inputThread.join();
		//outputThread.join();
	}

	void Game::gameOver() {
		m_graphics.drawTexture("Sprites/GameOver.bmp", 0, 0, 100, 100);
		m_graphics.renderGraphics();
		
		std::this_thread::sleep_for(std::chrono::seconds(1));
		addNewScore(m_matrix->getScore()); //TODO THE STRING HERE IS NOT NEEDED
		leaderBoard();
	}

	void Game::recieveUserInput(Direction& newDir) {

		while (m_running) {
			//inputManager.update();

			/* Escapes */
			if (m_inputManager.keyDown(SDL_SCANCODE_ESCAPE)) {
				m_running = false;
			}
			if (SDL_HasEvent(SDL_QUIT)) {
				m_running = false;
			}

			/* Actions */
			if (m_inputManager.keyDown(SDL_SCANCODE_X) && m_matrix->getSpecialFruits() > 0) {
				m_matrix->activateSpecialFruit();
			}

			/* Arrows - Input */
			if (m_inputManager.keyDown(SDL_SCANCODE_UP) || m_inputManager.keyDown(SDL_SCANCODE_W)) {
				newDir = Direction::up;

			} else if (m_inputManager.keyDown(SDL_SCANCODE_DOWN) || m_inputManager.keyDown(SDL_SCANCODE_S)) {
				newDir = Direction::down;

			} else if (m_inputManager.keyDown(SDL_SCANCODE_RIGHT) || m_inputManager.keyDown(SDL_SCANCODE_D)) {
				newDir = Direction::right;

			} else if (m_inputManager.keyDown(SDL_SCANCODE_LEFT) || m_inputManager.keyDown(SDL_SCANCODE_A)) {
				newDir = Direction::left;
			}

		}
	}

	void Game::moveSnake(Direction& direction) {
		if (direction == Direction::up) {
			m_matrix->moveSnakeUp();

		} else if (direction == Direction::down) {
			m_matrix->moveSnakeDown();

		} else if (direction == Direction::right) {
			m_matrix->moveSnakeRight();

		} else if (direction == Direction::left) {
			m_matrix->moveSnakeLeft();
		}

		std::cout << *m_matrix.get();
		
		updateGraphics(); 

		/*std::cout << "Notify before\n";
		graphicsCV.notify_one(); //ay im done, unlock
		std::cout << "Notify after\n";*/
	}

	void Game::renderGraphics() {
		//std::cout << "OUTPUT" << std::endl;
	}

	void Game::renderGameMargin() {

		m_graphics.clearScreen();
		m_graphics.drawTexture("Sprites/Green.bmp", 0, 0, 100, 100);
		m_graphics.writeText("Score: ", 50, 5, 60);
		m_graphics.renderGraphics();
	}

	inline unsigned Game::extractDigit(unsigned number, unsigned exponent) {
		return static_cast<unsigned>(number / pow(10, exponent)) % 10;
	}

	void Game::clearStats() {
		m_graphics.drawTexture("Sprites/Green.bmp", 70, 5, 15, 10);
		for (int i = 0; i < matrixSize; i++) {
			m_graphics.drawTexture("Sprites/Green.bmp", i, matrixSize + 1, matrixSize);
		}
	}

	void Game::updateGraphics() {
		unsigned specialFruits;
		unsigned score;
		auto matrixLayout = m_matrix->getLayout();

		//std::unique_lock<std::mutex> ul(m_graphicsLock);

		//std::cout << "update\n";
		//while (running) {
			//std::cout << "loop\n";
			//graphicsCV.wait(ul);
			//std::cout << "after wait\n";

			specialFruits = m_matrix->getSpecialFruits();
			score = m_matrix->getScore();


			//Every 5 score speed gets quicker
			if (score != 0 && score % 5 == 0) {
				m_speed -= 0.001;
			}

			for (Uint8 i = 0; i < matrixSize; i++) {
				for (Uint8 j = 0; j < matrixSize; j++) {
					auto cell = matrixLayout[i][j];
					char type = cell.getType();

					if (cell.hasChanged()) {
						std::string image = getImageFromType(type);
						m_graphics.drawTexture(image, j, i, matrixSize);
					}
				}
			}

			clearStats();
			//Setting stats
			for (int i = 0; i < specialFruits; i++) {
				m_graphics.drawTexture("Sprites/SpecialFruit.bmp", i, matrixSize + 1, matrixSize);
			}

			m_graphics.writeText(std::to_string(extractDigit(score, 2)), 70, 5, 60);
			m_graphics.writeText(std::to_string(extractDigit(score, 1)), 75, 5, 60);
			m_graphics.writeText(std::to_string(extractDigit(score, 0)), 80, 5, 60);
		//}
	}

	void Game::fillTextureBank() {
		//Textures
		/*m_graphics.createTexture("Sprites/GameOver.bmp", 'o');
		m_graphics.createTexture("Sprites/Green.bmp", 'g');
		m_graphics.createTexture("Sprites/TextBox.bmp", 'x');

		//Cells
		m_graphics.createTexture("Sprites/Black.bmp", '0');			//Background
		m_graphics.createTexture("Sprites/BrownBlock.bmp", 'w');		//Wall
		m_graphics.createTexture("Sprites/BrownBlock.bmp", 't');		//Temp Wall
		m_graphics.createTexture("Sprites/SnakeHead.bmp", 'h');		//Head
		m_graphics.createTexture("Sprites/SnakeBlock.bmp", 's');		//Tail
		m_graphics.createTexture("Sprites/InvertedHead.bmp", 'v');	//Inverted Head
		m_graphics.createTexture("Sprites/InvertedSnake.bmp", 'i');	//Inverted Tail
		m_graphics.createTexture("Sprites/Fruit.bmp", 'f');			//Red Fruit
		m_graphics.createTexture("Sprites/SpecialFruit.bmp", 'b');	//Blue Special Fruit*/
	}

	std::string Game::getImageFromType(char type) {
		std::string image = "Sprites/";

		switch (type) {
		case '0':
			image += "Black";
			break;
		case 'w':
			image += "BrownBlock";
			break;
		case 't': 
			image += "BrownBlock";
			break;
		case 'h':
			image += "SnakeHead";
			break;
		case 's':
			image += "SnakeBlock";
			break;
		case 'v':
			image += "InvertedHead";
			break;
		case 'i':
			image += "InvertedSnake";
			break;
		case 'f':
			image += "Fruit";
			break;
		case 'b':
			image += "SpecialFruit";
		}

		image += ".bmp";
		return image;
	}

	bool Game::mouseInsideArea(SDL_Rect rectangle) {
		int x = m_inputManager.getMouseX();
		int y = m_inputManager.getMouseY();

		return (
			x >= rectangle.x &&
			x <= rectangle.x + rectangle.w &&
			y >= rectangle.y &&
			y <= rectangle.y + rectangle.h
			);
	}

	void Game::leaderBoard() {

		std::vector<Score> scores;
		std::ifstream leaderBoard;
		std::string line;

		std::cout << "entering\n";

		m_graphics.clearScreen();
		m_graphics.drawTexture("Sprites/Green.bmp", 0, 0, 100, 100);
		m_graphics.writeText("Leaderboard:", 30, 10, 90);

		getFromLeaderBoard(scores);
		
		uint8_t yPercent = 30;
		int num = 1;
		for (int i = scores.size() - 1; i >= 0 && yPercent < 90; i--) {
			std::stringstream ss;
			ss << num << ": " << scores[i].getName() << " - " << scores[i].getScore() << " points.";
			m_graphics.writeText(ss.str(), 10, yPercent, 50);
			yPercent += 10;
			num++;
		}

		m_graphics.renderGraphics();

		waitForInput();
	}

	void Game::addNewScore(unsigned score) {

		std::vector<Score> scores;
		std::stringstream name;

		getFromLeaderBoard(scores);
		
		name << "Player " << scores.size();
		Score myScore(name.str(), score);

		//If scores are smae, the new will be placed beneath the first
		scores.insert(std::lower_bound(scores.begin(), scores.end(), myScore), myScore);
		writeToLeaderBoard(scores);
	}

	void Game::getFromLeaderBoard(std::vector<Score>& scores) {
		std::ifstream leaderBoard;
		std::string line;

		/* Opening leaderBoard file for reading old sorted results */
		leaderBoard.open("TextFiles/Leaderboard.txt");
		if (leaderBoard.is_open()) {
			while (std::getline(leaderBoard, line)) {

				size_t splitter = line.find_last_of(';');
				std::string tempName = line.substr(0, splitter);
				unsigned tempScore = std::stoi(line.substr(splitter + 1));

				//scores.emplace_back(tempName, tempScore);
				scores.emplace(scores.begin(), tempName, tempScore);
			}

			leaderBoard.close();
		} else {
			std::cout << "Unable to open leaderBoard file for input" << std::endl;
		}
	}

	void Game::writeToLeaderBoard(const std::vector<Score>& scores) {
		std::ofstream leaderBoard;
		
		/* Opening leaderBoard file for writing over with new sorted results */
		leaderBoard.open("TextFiles/Leaderboard.txt");
		if (leaderBoard.is_open()) {

			//for (auto it = scores.end(); it >= scores.begin(); --it) {
			for (int i = scores.size() - 1; i >= 0; i--) {
				leaderBoard << scores[i].getName() << ';' << scores[i].getScore() << '\n';
			}

			leaderBoard.close();
		} else {
			std::cout << "Unable to open leaderBoard file for output" << std::endl;
		}
	}

	void Game::waitForInput() {
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		while (!(
			m_inputManager.keyDown(SDL_SCANCODE_RETURN) ||
			m_inputManager.keyDown(SDL_SCANCODE_ESCAPE))) {
			//Her er det dumt å ha med musetast, da den leser musetasten nede når den entrer funksjonen Leaderboard

			m_inputManager.update();
		}
	}

	void Game::userTextInput() {
		//WE MIGHT ADD THIS BUT PERHAPS WE TAKE IT AWAY IF BECOMES TOO COMPLICATED
		SDL_StartTextInput();
		while (1) {
			m_inputManager.update();

			if (m_inputManager.keyDown(SDL_SCANCODE_ESCAPE)) {
				break;
			}

			if (m_inputManager.keyDown(SDL_TEXTINPUT)) {
				std::cout << "TEXTINPUT" << std::endl;
			}
		}
		SDL_StopTextInput();
	}
}
