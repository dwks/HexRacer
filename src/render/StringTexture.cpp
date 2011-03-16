#include "StringTexture.h"
#include "math/Values.h"

namespace Project {
namespace Render {

	StringTexture::StringTexture() {
		textureID = 0;
	}

	StringTexture::~StringTexture() {
		clearTexture();
	}

	void StringTexture::clearTexture() {
		if (hasTexture())
			glDeleteTextures(1, &textureID);
		textureID = 0;
		textureString = "";
	}

	void StringTexture::setString(const std::string& str,
		TTF_Font *font,
		const OpenGL::Color& text_color,
		bool power_of_two_size) {

			if (str == textureString)
				return;

			clearTexture();

			if (str.empty())
				return;

			textureString = std::string(str);

			SDL_Color c;
			c.r = text_color.getRedi();
			c.g = text_color.getGreeni();
			c.b = text_color.getBluei();

			// note: we rely on this function creating 32-bit images,
			// as its documentation describes.
			SDL_Surface *first = TTF_RenderText_Blended(font, str.c_str(), c);
			SDL_SetAlpha(first, SDL_SRCALPHA || SDL_RLEACCEL, 128);
		    
			originalWidth = first->w;
			originalHeight = first->h;
			if (power_of_two_size) {
				width = Math::nextPowerOf2(originalWidth);
				height = Math::nextPowerOf2(originalHeight);
			}
			else {
				width = originalWidth;
				height = originalHeight;
			}
		    
			SDL_Surface *second = SDL_CreateRGBSurface(
				SDL_SWSURFACE, width, height,
				32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
			SDL_SetAlpha(second, SDL_SRCALPHA || SDL_RLEACCEL, 128);
		    
			// copy data from first to second, translating to new format
			SDL_BlitSurface(first, NULL, second, NULL);
		    
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
		    
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    
			// we assume this copies the pixels from second, and later free second
			glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA, 
				GL_UNSIGNED_BYTE, second->pixels);
		    
			glBindTexture(GL_TEXTURE_2D, NULL);

			SDL_FreeSurface(first);
			SDL_FreeSurface(second);

	}

}  // namespace Render
}  // namespace Project
