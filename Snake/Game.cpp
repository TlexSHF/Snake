#include "Game.h"

namespace snake {

	Game::Game(size_t windowSize) :
		m_matrix(),
		m_windowSize(windowSize),
		m_inputManager(m_inputManager.getInstance()),
		m_graphics("The amazing Snake!", windowSize, 150) {
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
					loadingScreen();
					renderGameMargin();
					startGame();
				}

				if (mouseInsideArea(txtBox2)) {
					leaderBoard();
				}
			}
		}
	}

	void Game::loadingScreen() {
		updatePlayerName();

		m_graphics.clearScreen();

		m_graphics.writeText("Quick tips!", 5, 5, 90);
		m_graphics.writeText("Move with: WASD or the ARROW KEYS", 5, 20, 50);
		m_graphics.writeText("Move in any direction, except for", 5, 26, 50);
		m_graphics.writeText("opposite of where you are heading", 5, 30, 50);
		m_graphics.writeText("RED BERRY: Makes you bigger!", 5, 40, 50);
		m_graphics.writeText("BLUE BERRY: Save them for later...", 5, 45, 50);
		m_graphics.writeText("BROWN WALL: Hurts alot!", 5, 50, 50);
		m_graphics.writeText("Press [X] to use your blue berries, ", 5, 60, 50);
		m_graphics.writeText("and become insvinsible!", 5, 65, 50);
		m_graphics.writeText("Some brown walls will be crushed,", 5, 70, 50);
		m_graphics.writeText("when hitting them with invinsibility", 5, 75, 50);

		m_graphics.writeText("Loading game...", 5, 90, 40);
		m_graphics.renderGraphics();
		std::this_thread::sleep_for(std::chrono::seconds(20));

		m_graphics.clearScreen();
		m_graphics.writeText("You are " + m_playerName, 5, 40, 80);
		m_graphics.renderGraphics();
		std::this_thread::sleep_for(std::chrono::seconds(2));

		m_graphics.clearScreen();
		m_graphics.writeText("3", 40, 40, 130);
		m_graphics.renderGraphics();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_graphics.clearScreen();
		m_graphics.writeText("2", 40, 40, 130);
		m_graphics.renderGraphics();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_graphics.clearScreen();
		m_graphics.writeText("1", 40, 40, 130);
		m_graphics.renderGraphics();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	void Game::startGame() {
		m_running = true;
		m_speed = 0.3;

		Direction newDir = Direction::up;
		Direction direction = newDir;

		std::thread inputThread([this, &newDir] { recieveUserInput(newDir); });

		m_matrix = std::make_unique<Matrix<matrixSize>>();

		auto timeCount = m_time.now();

		updateGraphics();

		while (m_running) {

			m_graphics.renderGraphics();
			m_inputManager.update();

			//Will only move once every {speed} seconds
			std::chrono::duration<double> diff = m_time.now() - timeCount;
			if (diff.count() > m_speed) {
				timeCount = m_time.now();

				//Will not move in the direction opposite of the current
				if (!(newDir == !direction)) {
					direction = newDir;
				}

				moveSnake(direction);
			}
			if (m_matrix->isGameOver()) {
				gameOver();
				m_running = false;
			}
			
			
		}
		inputThread.join();
	}

	void Game::gameOver() {
		m_graphics.drawTexture("Sprites/GameOver.bmp", 0, 0, 100, 100);
		m_graphics.renderGraphics();
		
		std::this_thread::sleep_for(std::chrono::seconds(1));
		addNewScore(m_matrix->getScore());
		leaderBoard();
	}

	void Game::recieveUserInput(Direction& newDir) {

		while (m_running) {

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
	}

	void Game::renderGameMargin() {

		m_graphics.clearScreen();
		m_graphics.drawTexture("Sprites/Green.bmp", 0, 0, 100, 100);
		m_graphics.writeText("Score: ", 50, 5, 60);
		m_graphics.writeText("X to use:", 0, 95, 30);
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

		for (int i = 0; i < specialFruits; i++) {
			m_graphics.drawTexture("Sprites/SpecialFruit.bmp", i, matrixSize + 1, matrixSize);
		}

		m_graphics.writeText(std::to_string(extractDigit(score, 2)), 70, 5, 60);
		m_graphics.writeText(std::to_string(extractDigit(score, 1)), 75, 5, 60);
		m_graphics.writeText(std::to_string(extractDigit(score, 0)), 80, 5, 60);
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

		getFromLeaderBoard(scores);

		Score myScore(m_playerName, score);

		scores.insert(std::lower_bound(scores.begin(), scores.end(), myScore), myScore);
		writeToLeaderBoard(scores);
	}

	void Game::updatePlayerName() {
		std::vector<Score> scores;
		std::stringstream name;

		getFromLeaderBoard(scores);

		name << "Player " << scores.size();
		m_playerName = name.str();
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

			for (int i = scores.size() - 1; i >= 0; i--) {
				leaderBoard << scores[i].getName() << ';' << scores[i].getScore() << '\n';
			}

			leaderBoard.close();
		} else {
			std::cout << "Unable to open leaderBoard file for output" << std::endl;
		}
	}

	void Game::waitForInput() {
		while (!(
			m_inputManager.keyDown(SDL_SCANCODE_RETURN) ||
			m_inputManager.keyDown(SDL_SCANCODE_ESCAPE))) {

			m_inputManager.update();
		}
	}
}
