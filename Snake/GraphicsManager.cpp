#include <iostream>
#include "GraphicsManager.h"

/* ------------- PUBLIC ------------- */
GraphicsManager::GraphicsManager(std::string name, size_t size) :
	GraphicsManager(name, size, size) {}

GraphicsManager::GraphicsManager(std::string name, size_t width, size_t height) : 
	m_windowName(name),
	m_windowWidth(width), 
	m_windowHeight(height) {

	SDL_Init(SDL_INIT_VIDEO);

	if (createWindow() == ERROR) return;
	if (createRenderer() == ERROR) return;
}

GraphicsManager::~GraphicsManager() {
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void GraphicsManager::createTexture(std::string image) {
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

		m_textures.push_back(texture);
	}
}

void GraphicsManager::renderTexture(unsigned index, unsigned x, unsigned y, size_t gridSize) {

	x = x * (m_windowWidth / gridSize);
	y = y * (m_windowHeight / gridSize);


	SDL_Rect coords;
	coords.x = x;
	coords.y = y;
	coords.w = m_windowWidth / gridSize;
	coords.h = m_windowHeight / gridSize;

	SDL_RenderCopy(
		m_renderer,
		m_textures[index],
		nullptr,
		&coords
	);
}

void GraphicsManager::renderGraphics() {
	SDL_RenderPresent(m_renderer);
	//SDL_RenderClear(m_renderer);
}

std::vector<SDL_Texture*> GraphicsManager::getTextures() {
	return m_textures;
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
