#include "TextureManager.h"

namespace snake {
	SDL_Texture* TextureManager::getTextTexture(std::string text, size_t size, SDL_Renderer* renderer, SDL_Window* window) {
		std::string key = makeKey(text, size);

		if (m_textures.count(key) > 0) {
			return m_textures[key];
		} else {
			createTextTexture(text, size, renderer, window);
			return m_textures[key];
		}
	}
	SDL_Texture* snake::TextureManager::getTexture(std::string image, SDL_Renderer* renderer, SDL_Window* window) {
		if (m_textures.count(image) > 0) {
			return m_textures[image];
		} else {
			createTexture(image, renderer, window);
			return m_textures[image];
		}
	}

	std::string TextureManager::makeKey(std::string text, size_t size) {
		std::stringstream key;
		key << text << ':' << size;
		return key.str();
	}

	void TextureManager::createTextTexture(std::string text, size_t size, SDL_Renderer* renderer, SDL_Window* window) {
		std::string key = makeKey(text, size);
		TTF_Font* font = TTF_OpenFont("Fonts/NewTegomin-Regular.ttf", size);

		if (font == nullptr) {
			std::cerr << "Failed to load font: "
				<< SDL_GetError() << std::endl;
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();

		} else {

			SDL_Color color = { 0, 120, 0 };
			SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);

			//m_texts.emplace_back(text, size, texture);
			m_textures.insert({ key, texture });
		}
	}

	void snake::TextureManager::createTexture(std::string image, SDL_Renderer* renderer, SDL_Window* window) {
		SDL_Surface* surface = SDL_LoadBMP(image.c_str());

		if (surface == nullptr) {
			std::cerr << "Failed to load image: "
				<< SDL_GetError() << std::endl;
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();

		} else {
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);

			//Insertion doesn't override the first texture written
			m_textures.insert({ image, texture });
		}
	}
}
