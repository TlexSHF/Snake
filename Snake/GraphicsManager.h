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
	empty,
	up,
	down,
	right,
	left
};

class GraphicsManager {
public:
	GraphicsManager();
	~GraphicsManager();

	std::vector<SDL_Texture*> m_textures;

	int updateTexture(std::string image);
	//void createSprite(SDL_Texture* texture, SDL_Rect& coords, std::string image, int x, int y);
	void setX(int x);
	void setY(int y);

	SDL_Window* getWindow();
	SDL_Renderer* getRenderer();
	SDL_Texture* getTexture();
	SDL_Rect getCoords();

private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	SDL_Texture* m_texture = nullptr;
	SDL_Rect m_coords;

	bool m_exitFailure = false;

	int createWindow();
	int createRenderer();
	int setRect(SDL_Surface* surface);
	//SDL_Rect createCoords(SDL_Surface* surface, int x, int y);
};

#endif
