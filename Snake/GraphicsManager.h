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

/* USING THE GRAPHICSMANAGER
	1) Call Constructor#1 for a square window, or Constructor#2 for window of custom dimensions.
	2) Always begin a render by clearing the screen with clearScreen().
	3) Then apply Graphics by calling either drawTexure() or writeText().
		3.5) drawTexture() has two implementations: 
				- One based on (x,y) coordinates (Good for matrix usages)
				- One based on % coordinates of the screen, and % size of the image. (Good for specific placements)
	4) To show the image you have created, call renderGraphics().
*/

namespace managers {

	class GraphicsManager {
	public:
		GraphicsManager(std::string name, size_t size, size_t margin);
		GraphicsManager(std::string name, size_t width, size_t height, size_t margin);
		~GraphicsManager();

		void clearScreen(); //This is called first
		void renderGraphics(); //This is called last

		void drawTexture(std::string image, unsigned x, unsigned y, size_t gridSize);
		SDL_Rect drawTexture(std::string image, double xPercent, double yPercent, double wPercent, double hPercent);
		void writeText(std::string text, double xPercent, double yPercent, size_t size);

	private:
		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;
		managers::TextureManager m_textureManager;

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
