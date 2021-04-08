#ifndef TEXTTEXTURE_H
#define TEXTTEXTURE_H

#include <string>
#include <SDL.h>

namespace snake {

	struct TextTexture {
	public:
		TextTexture(std::string text, size_t size, SDL_Texture* texture) :
			m_text(text), m_size(size), m_texture(texture) {}

		std::string getText() const;
		size_t getSize() const;
		SDL_Texture* getTexture() const;

	private:
		std::string m_text;
		size_t m_size;
		SDL_Texture* m_texture;
	};
}

#endif;