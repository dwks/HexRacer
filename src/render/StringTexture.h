#ifndef PROJECT_RENDER__STRING_TEXTURE_H
#define PROJECT_RENDER__STRING_TEXTURE_H

#include "TextureObject.h"
#include "opengl/OpenGL.h"
#include "opengl/Color.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

namespace Project {
namespace Render {

	class StringTexture
		: public TextureObject {
	private:
		GLuint textureID;
		int width;
		int height;
		int originalWidth;
		int originalHeight;
		std::string textureString;
	public:

		StringTexture();
		~StringTexture();

		GLuint getTextureID() const { return textureID; }
		void clearTexture();

		int getWidth() const { return width; }
		int getHeight() const { return height; }
		int getOriginalWidth() const { return originalWidth; }
		int getOriginalHeight() const { return originalHeight; }

		void setString(const std::string& str, TTF_Font *font, const OpenGL::Color& text_color = OpenGL::Color::WHITE, bool power_of_two_size = false);
		const std::string& getString() const { return textureString; }

		bool getVFlip() const { return true; }
	};

}  // namespace Render
}  // namespace Project

#endif
