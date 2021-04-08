#include "TextTexture.h"

namespace snake {

	std::string snake::TextTexture::getText() const {
		return m_text;
	}

	size_t snake::TextTexture::getSize() const {
		return m_size;
	}

	SDL_Texture* snake::TextTexture::getTexture() const {
		return m_texture;
	}

}