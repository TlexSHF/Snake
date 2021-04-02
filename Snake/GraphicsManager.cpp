#include <iostream>
#include "GraphicsManager.h"

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

void GraphicsManager::readyTexture(char indexTag, double xPercent, double yPercent, double wPercent, double hPercent) {

	SDL_Rect coords;
	coords.x = m_windowWidth * (xPercent / 100);
	coords.y = m_windowHeight * (yPercent / 100);
	coords.w = m_windowWidth * (wPercent / 100);
	coords.h = m_windowHeight * (hPercent / 100);

	renderCopy(indexTag, coords, false);
}

void GraphicsManager::readyText(char indexTag, double xPercent, double yPercent) {
	
	SDL_Rect coords;
	coords.x = m_windowWidth * (xPercent / 100);
	coords.y = m_windowHeight * (yPercent / 100);

	renderCopy(indexTag, coords, true);
}

void GraphicsManager::createText(std::string text, size_t size, char tag) {

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

		m_texts.emplace_back(texture, tag);
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

std::vector<TexturePair> GraphicsManager::getTexts() {
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

void GraphicsManager::renderCopy(char indexTag, SDL_Rect& coords, bool isText) {
	size_t size;
	std::vector<TexturePair> vec;
	int textW = 0;
	int textH = 0;

	if (isText) {
		size = m_texts.size();
		vec = m_texts;
	} else {
		size = m_textures.size();
		vec = m_textures;
	}

	for (int i = 0; i < size; i++) {

		if (vec[i].tag == indexTag) {
			if (isText) {
				SDL_QueryTexture(vec[i].texture, NULL, NULL, &textW, &textH);
				coords.w = textW;
				coords.h = textH;
			}

			SDL_RenderCopy(
				m_renderer,
				vec[i].texture,
				nullptr,
				&coords
			);
			break;
		}
	}
	
}
