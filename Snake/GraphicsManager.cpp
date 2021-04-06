#include <iostream>
#include "GraphicsManager.h"

namespace snake {

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

	void GraphicsManager::createTexture(std::string image, char tag) {
		SDL_Surface* surface = SDL_LoadBMP(image.c_str());

		if (surface == nullptr) {
			std::cerr << "Failed to load image: "
				<< SDL_GetError() << std::endl;
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			SDL_Quit();

		} else {
			SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
			SDL_FreeSurface(surface);

			m_textures.emplace_back(texture, tag);
		}
	}

	void GraphicsManager::readyTexture(char indexTag, unsigned x, unsigned y, size_t gridSize) {

		SDL_Rect coords;
		coords.x = x * m_baseWidth / gridSize + m_margin;
		coords.y = y * m_baseHeight / gridSize + m_margin;
		coords.w = m_baseWidth / gridSize;
		coords.h = m_baseHeight / gridSize;

		renderCopy(indexTag, coords, false);
	}

	SDL_Rect GraphicsManager::readyTexture(char indexTag, double xPercent, double yPercent, double wPercent, double hPercent) {

		SDL_Rect coords;
		coords.x = m_windowWidth * (xPercent / 100);
		coords.y = m_windowHeight * (yPercent / 100);
		coords.w = m_windowWidth * (wPercent / 100);
		coords.h = m_windowHeight * (hPercent / 100);

		renderCopy(indexTag, coords, false);
		return coords;
	}

	void GraphicsManager::writeText(std::string text, double xPercent, double yPercent, size_t size) {
		int textIndex = existsInTexts(text, size);

		if (textIndex == -1) {
			//Text not found, creating new
			createText(text, size);
			readyText(m_texts.back().texture, xPercent, yPercent);
		} else {
			readyText(m_texts[textIndex].texture, xPercent, yPercent);
		}
	}

	void GraphicsManager::renderGraphics() {
		SDL_RenderPresent(m_renderer);
		//SDL_RenderClear(m_renderer);
	}

	void GraphicsManager::clearScreen() {
		SDL_RenderClear(m_renderer);
	}

	std::vector<TexturePair> GraphicsManager::getTextures() {
		return m_textures;
	}

	std::vector<TextTexture> GraphicsManager::getTexts() {
		return m_texts;
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

	void GraphicsManager::createText(std::string text, size_t size) {

		TTF_Font* font = TTF_OpenFont("Fonts/NewTegomin-Regular.ttf", size);

		if (font == nullptr) {
			std::cerr << "Failed to load font: "
				<< SDL_GetError() << std::endl;
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			SDL_Quit();

		} else {

			SDL_Color color = { 0, 120, 0 };
			SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
			SDL_FreeSurface(surface);

			m_texts.emplace_back(text, size, texture);
		}
	}

	void GraphicsManager::readyText(SDL_Texture* texture, double xPercent, double yPercent) {
		int textW = 0;
		int textH = 0;

		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);

		SDL_Rect coords;
		coords.x = m_windowWidth * (xPercent / 100);
		coords.y = m_windowHeight * (yPercent / 100);
		coords.w = textW;
		coords.h = textH;

		//Render
		SDL_RenderCopy(
			m_renderer,
			texture,
			nullptr,
			&coords
		);
	}

	void GraphicsManager::renderCopy(char indexTag, SDL_Rect& coords, bool isText) {
		size_t size = m_textures.size();

		for (int i = 0; i < size; i++) {

			if (m_textures[i].tag == indexTag) {


				SDL_RenderCopy(
					m_renderer,
					m_textures[i].texture,
					nullptr,
					&coords
				);
				break;
			}
		}

	}

	int GraphicsManager::existsInTexts(std::string text, size_t size) {
		int textsSize = m_texts.size();
		for (int i = 0; i < textsSize; i++) {
			if (m_texts[i].text == text && m_texts[i].size == size) {
				return i;
			}
		}
		return -1;
	}
}