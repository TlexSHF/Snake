#ifndef TEXTTEXTURE_H
#define TEXTTEXTURE_H

#include <string>
#include <SDL.h>

namespace snake {

	struct TextTexture {
		TextTexture(std::string text, size_t size, SDL_Texture* texture) :
			text(text), size(size), texture(texture) {}

		std::string text;
		size_t size;
		SDL_Texture* texture;
	};
}

#endif;