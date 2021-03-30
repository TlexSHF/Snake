#ifndef WINDOW_H
#define WINDOW_H
#define SDL_MAIN_HANDLED

#define OK 0
#define ERROR 1

#include <string>
#include <SDL.h>
#include <vector>
#include "InputManager.h"

enum class Direction {
	stop,
	up,
	down,
	right,
	left
};

class GraphicsManager {
public:
	GraphicsManager(std::string name, size_t size);
	GraphicsManager(std::string name, size_t width, size_t height);
	~GraphicsManager();

	void createTexture(std::string image);
	void renderTexture(unsigned index, unsigned x, unsigned y, size_t gridSize);
	void renderGraphics();

	std::vector<SDL_Texture*> getTextures();

private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	std::vector<SDL_Texture*> m_textures;

	std::string m_windowName;
	size_t m_windowWidth;
	size_t m_windowHeight;

	bool m_exitFailure = false;

	int createWindow();
	int createRenderer();
};

#endif
