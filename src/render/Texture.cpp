#include "Texture.h"
#include "SDL_image.h"
#include "SDL.h"

namespace Project {
namespace Render {

	Texture::Texture(std::string _name, std::string color_map_filename, std::string normal_map_filename) {
		name = _name;
		colorMap = loadTexture(color_map_filename);
		normalMap = loadTexture(normal_map_filename);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &colorMap);
		glDeleteTextures(1, &normalMap);
	}

	std::string Texture::getName() const {
		return name;
	}

	bool Texture::hasColorMap() const {
		return (colorMap > 0);
	}

	GLuint Texture::getColorMap() const {
		return colorMap;
	}

	bool Texture::hasNormalMap() const {
		return (normalMap > 0);
	}

	GLuint Texture::getNormalMap() const {
		return normalMap;
	}

	GLuint Texture::loadTexture(std::string filename,
		GLint wrap_s, GLint wrap_t,
		GLint filter_s, GLint filter_t) {

		SDL_Surface* texture = IMG_Load(filename.c_str());
		if (texture) {
			GLuint tex_id;
			glGenTextures(1, &tex_id);

			//Get the right image format
			GLint image_format = GL_RGB; 
			switch (texture->format->BytesPerPixel) {
				case 1:
					image_format = GL_LUMINANCE;
					;break;
				case 3:
					image_format = GL_RGB;
					;break;
				case 4:
					image_format = GL_RGBA;
					;break;
			}
	
			glBindTexture(GL_TEXTURE_2D, tex_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_t);

			glTexImage2D(GL_TEXTURE_2D, 0, image_format, texture->w, texture->h, 0, image_format, GL_UNSIGNED_BYTE, texture->pixels);
			SDL_FreeSurface(texture);
			return tex_id;
		}
		else
			return 0;

	}


}  // namespace Render
}  // namespace Project
