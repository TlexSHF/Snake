#include <iostream>
#include "GraphicsManager.h"

namespace managers {

	/* ------------- PUBLIC ------------- */
	GraphicsManager::GraphicsManager(std::string name, size_t size, size_t margin) :
		GraphicsManager(name, size, size, margin) {}

	GraphicsManager::GraphicsManager(std::string name, size_t width, size_t height, size_t margin) :
		m_windowName(name),
		m_windowWidth(width),
		m_windowHeight(height),
		m_margin(margin),
		m_baseWidth(width - margin * 2),
		m_baseHeight(height - margin * 2) {

		SDL_Init(SDL_INIT_VIDEO);
		TTF_Init();

		if (createWindow() == ERROR) return;
		if (createRenderer() == ERROR) return;
	}

	GraphicsManager::~GraphicsManager() {
		TTF_Quit();
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	void GraphicsManager::renderGraphics() {
		SDL_RenderPresent(m_renderer);
	}

	void GraphicsManager::clearScreen() {
		SDL_RenderClear(m_renderer);
	}

	void GraphicsManager::drawTexture(std::string image, unsigned x, unsigned y, size_t gridSize) {
		
		SDL_Texture* texture = m_textureManager.getTexture(image, m_renderer, m_window);

		SDL_Rect coords;
		coords.x = static_cast<int>(x * m_baseWidth / gridSize + m_margin);
		coords.y = static_cast<int>(y * m_baseHeight / gridSize + m_margin);
		coords.w = m_baseWidth / gridSize;
		coords.h = m_baseHeight / gridSize;

		renderCopy(texture, coords);
	}

	SDL_Rect GraphicsManager::drawTexture(std::string image, double xPercent, double yPercent, double wPercent, double hPercent) {

		SDL_Texture* texture = m_textureManager.getTexture(image, m_renderer, m_window);

		SDL_Rect coords;
		coords.x = static_cast<int>(m_windowWidth * (xPercent / 100));
		coords.y = static_cast<int>(m_windowHeight * (yPercent / 100));
		coords.w = static_cast<int>(m_windowWidth * (wPercent / 100));
		coords.h = static_cast<int>(m_windowHeight * (hPercent / 100));

		renderCopy(texture, coords);
		return coords;
	}

	void GraphicsManager::writeText(std::string text, double xPercent, double yPercent, size_t size) {

		SDL_Texture* texture = m_textureManager.getTextTexture(text, size, m_renderer, m_window);
		
		int textW = 0;
		int textH = 0;

		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);

		SDL_Rect coords;
		coords.x = m_windowWidth * (xPercent / 100);
		coords.y = m_windowHeight * (yPercent / 100);
		coords.w = textW;
		coords.h = textH;

		renderCopy(texture, coords);
	}


	/* ------------- PRIVATE ------------- */
	int GraphicsManager::createWindow() {

		m_window = SDL_CreateWindow(
			m_windowName.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			m_windowWidth,
			m_windowHeight,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
		);

		if (m_window == nullptr) {
			std::cerr << "failed to create window: "
				<< SDL_GetError() << std::endl;
			SDL_Quit();
			return ERROR;
		}

		return OK;
	}

	int GraphicsManager::createRenderer() {
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

		if (m_renderer == nullptr) {
			std::cerr << "failed to create renderer: "
				<< SDL_GetError() << std::endl;
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return ERROR;
		}

		return OK;
	}

	void GraphicsManager::renderCopy(SDL_Texture* texture, SDL_Rect& coords) {

		SDL_RenderCopy(
			m_renderer,
			texture,
			nullptr,
			&coords
		);
	}
}