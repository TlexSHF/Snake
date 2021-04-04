#ifndef TEXTUREPAIR_H
#define TEXTUREPAIR_H

#include <SDL.h>

struct TexturePair {
	TexturePair(SDL_Texture* texture, char tag) :
		texture(texture), tag(tag) {}

	SDL_Texture* texture;
	char tag;
};

#endif
