#ifndef WINDOW_H
#define WINDOW_H
#define SDL_MAIN_HANDLED

#define OK 0
#define ERROR 1

#include <string>
#include <SDL.h>
#include <vector>
#include "TexturePair.h"

class GraphicsManager {
public:
	GraphicsManager(std::string name, size_t size, size_t margin);
	GraphicsManager(std::string name, size_t width, size_t height, size_t margin);
	~GraphicsManager();

	void createTexture(std::string image, char tag);
	void readyTexture(char indexTag, unsigned x, unsigned y, size_t gridSize);
	void readyTexture(char indexTag, double xPercent, double yPercent, double wPercent, double hPercent);

	void renderGraphics();
	void clearScreen();

	std::vector<TexturePair> getTextures();

private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	std::vector<TexturePair> m_textures;

	std::string m_windowName;
	size_t m_windowWidth;
	size_t m_windowHeight;
	size_t m_margin;

	bool m_exitFailure = false;

	int createWindow();
	int createRenderer();
	void renderCopy(char indexTag, SDL_Rect& coords);
};

#endif
