#ifndef WINDOW_H
#define WINDOW_H
#define SDL_MAIN_HANDLED

#define OK 0
#define ERROR 1

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "TexturePair.h"
#include "TextTexture.h"

namespace snake {

	class GraphicsManager {
	public:
		GraphicsManager(std::string name, size_t size, size_t margin);
		GraphicsManager(std::string name, size_t width, size_t height, size_t margin);
		~GraphicsManager();

		//TODO: Make textures work like texts with dynamically adding vector
		void createTexture(std::string image, char tag);
		void readyTexture(char indexTag, unsigned x, unsigned y, size_t gridSize);
		SDL_Rect readyTexture(char indexTag, double xPercent, double yPercent, double wPercent, double hPercent);

		void writeText(std::string text, double xPercent, double yPercent, size_t size);

		void renderGraphics();
		void clearScreen();

		/* Getters */
		std::vector<TexturePair> getTextures();
		std::vector<TextTexture> getTexts();

	private:
		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;
		std::vector<TexturePair> m_textures;
		//std::vector<TexturePair> m_texts;
		std::vector<TextTexture> m_texts;

		std::string m_windowName;
		size_t m_windowWidth;
		size_t m_windowHeight;
		size_t m_margin;
		unsigned m_baseWidth;
		unsigned m_baseHeight;

		bool m_exitFailure = false;

		int createWindow();
		int createRenderer();
		void createText(std::string text, size_t size);
		void readyText(SDL_Texture* texture, double xPercent, double yPercent);
		void renderCopy(char indexTag, SDL_Rect& coords, bool isText);
		int existsInTexts(std::string text, size_t size);
	};
}

#endif
