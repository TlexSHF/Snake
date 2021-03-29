#include <iostream>
#include "GraphicsManager.h"

/* ------------- PUBLIC ------------- */
GraphicsManager::GraphicsManager() {

	SDL_Init(SDL_INIT_VIDEO);

	if (createWindow() == ERROR) return;
	if (createRenderer() == ERROR) return;
	if (updateTexture("SnakeHead.bmp") == ERROR) return;

	//Filling up the texture bank
	if (createTexture("Black.bmp") == ERROR) return;
	if (createTexture("BrownBlock.bmp") == ERROR) return;
	if (createTexture("SnakeBlock.bmp") == ERROR) return;
	if (createTexture("Fruit.bmp") == ERROR) return;	
	if (createTexture("SnakeHead.bmp") == ERROR) return;
	if (createTexture("GameOver.bmp") == ERROR) return;
}

GraphicsManager::~GraphicsManager() {
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

int GraphicsManager::updateTexture(std::string image) {
	if (m_texture)
		SDL_DestroyTexture(m_texture);

	SDL_Surface* surface = SDL_LoadBMP(image.c_str());

	if (surface == nullptr) {
		std::cerr << "Failed to load image: "
			<< SDL_GetError() << std::endl;
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return ERROR;
	}

	m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);

	if (setRect(surface) == ERROR) {
		return ERROR;
	}

	SDL_FreeSurface(surface);
	return OK;
}

int GraphicsManager::createTexture(std::string image) {
	SDL_Surface* surface = SDL_LoadBMP(image.c_str());

	if (surface == nullptr) {
		std::cerr << "Failed to load image: "
			<< SDL_GetError() << std::endl;
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return ERROR;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_FreeSurface(surface);

	m_textures.push_back(texture);
	return OK;
}

void GraphicsManager::renderTexture(unsigned index, unsigned x, unsigned y) {

	SDL_Rect coords;
	coords.x = x;
	coords.y = y;
	coords.w = 100;
	coords.h = 100;

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

void GraphicsManager::setX(int x) {
	m_coords.x = x;
}

void GraphicsManager::setY(int y) {
	m_coords.y = y;
}

SDL_Window* GraphicsManager::getWindow() {
	return m_window;
}

SDL_Renderer* GraphicsManager::getRenderer() {
	return m_renderer;
}

std::vector<SDL_Texture*> GraphicsManager::getTextures() {
	return m_textures;
}

SDL_Texture* GraphicsManager::getTexture() {
	return m_texture;
}

SDL_Rect GraphicsManager::getCoords() {
	return m_coords;
}

/* ------------- PRIVATE ------------- */
int GraphicsManager::createWindow() {

	m_window = SDL_CreateWindow(
		"The amazing window!",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1000,
		1000,
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

int GraphicsManager::setRect(SDL_Surface* surface) {
	if (surface == nullptr) {
		return ERROR;
	}

	if (m_coords.h == 0) {

		m_coords.h = surface->h / 2;
		m_coords.w = surface->w / 2;
		m_coords.x = 0;
		m_coords.y = 0;
	}
	return OK;
}
