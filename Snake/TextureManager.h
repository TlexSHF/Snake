#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <iostream>
#include <map>

namespace managers {

	class TextureManager {
	public:
		SDL_Texture* getTexture(std::string image, SDL_Renderer* renderer, SDL_Window* window);
		SDL_Texture* getTextTexture(std::string text, size_t size, SDL_Renderer* renderer, SDL_Window* window);

	private:
		std::map<std::string, SDL_Texture*> m_textures;

		std::string makeKey(std::string text, size_t size);

		void createTexture(std::string image, SDL_Renderer* renderer, SDL_Window* window);
		void createTextTexture(std::string text, size_t size, SDL_Renderer* renderer, SDL_Window* window);
	};

}

#endif