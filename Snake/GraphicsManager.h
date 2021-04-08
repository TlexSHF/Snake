#ifndef WINDOW_H
#define WINDOW_H
#define SDL_MAIN_HANDLED

#define OK 0
#define ERROR 1

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "TextureManager.h"

namespace snake {

	class GraphicsManager {
	public:
		GraphicsManager(std::string name, size_t size, size_t margin);
		GraphicsManager(std::string name, size_t width, size_t height, size_t margin);
		~GraphicsManager();

		void renderGraphics();
		void clearScreen();

		void drawTexture(std::string image, unsigned x, unsigned y, size_t gridSize);
		SDL_Rect drawTexture(std::string image, double xPercent, double yPercent, double wPercent, double hPercent);
		void writeText(std::string text, double xPercent, double yPercent, size_t size);

	private:
		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;
		TextureManager m_textureManager;

		std::string m_windowName;
		size_t m_windowWidth;
		size_t m_windowHeight;
		size_t m_margin;
		unsigned m_baseWidth;
		unsigned m_baseHeight;

		bool m_exitFailure = false;

		int createWindow();
		int createRenderer();
		void renderCopy(SDL_Texture* texture, SDL_Rect& coords);
	};
}

#endif
