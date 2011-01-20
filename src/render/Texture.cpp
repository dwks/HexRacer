#include "Texture.h"
#include <SDL_image.h>
#include <SDL.h>

namespace Project {
namespace Render {

	Texture::Texture(std::string _name, std::string color_map_filename, std::string normal_map_filename) {
		name = _name;
		colorMap = loadTexture2D(color_map_filename);
		normalMap = loadTexture2D(normal_map_filename);
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

	GLuint Texture::loadTexture2D(std::string filename,
		GLint wrap_s, GLint wrap_t,
		GLint filter_min, GLint filter_mag) {

		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (surface) {
			GLuint tex_id;
			glGenTextures(1, &tex_id);

			glBindTexture(GL_TEXTURE_2D, tex_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);

			GLint image_format = toGLFormat(surface->format->BytesPerPixel);
			glTexImage2D(GL_TEXTURE_2D,
				0,
				image_format,
				surface->w,
				surface->h,
				0,
				image_format,
				GL_UNSIGNED_BYTE,
				surface->pixels);

			SDL_FreeSurface(surface);
			
			return tex_id;
		}
		else
			return 0;

	}

	GLuint Texture::loadTextureCube(
		std::string positive_x_file,
		std::string negative_x_file,
		std::string positive_y_file,
		std::string negative_y_file,
		std::string positive_z_file,
		std::string negative_z_file,
		GLint filter_min, GLint filter_mag) {

		GLuint tex_id;
		glGenTextures(1, &tex_id);

		glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter_min);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter_mag);

		for (int i = 0; i < 6; i++) {
			std::string filename;
			switch (i) {
				case 0: filename = positive_x_file; break;
				case 1: filename = negative_x_file; break;
				case 2: filename = positive_y_file; break;
				case 3: filename = negative_y_file; break;
				case 4: filename = positive_z_file; break;
				default: filename = negative_z_file; break;
			}

			SDL_Surface* surface = IMG_Load(filename.c_str());
			if (surface) {
				GLint image_format = toGLFormat(surface->format->BytesPerPixel);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
				0,
				image_format,
				surface->w,
				surface->h,
				0,
				image_format,
				GL_UNSIGNED_BYTE,
				surface->pixels);
				SDL_FreeSurface(surface);
			}
		}

		return tex_id;
	}


	GLint Texture::toGLFormat(int bytes_per_pixel) {
		switch (bytes_per_pixel) {
			case 1:
				return GL_LUMINANCE;
			case 3:
				return GL_RGB;
			case 4:
				return GL_RGBA;
			default:
				return GL_RGB;
		}
	}



	/*
	void Texture::loadSurfaceToTexture(SDLSurface* surface, GLenum target) {
		//Get the right image format
		
	}
	*/
}  // namespace Render
}  // namespace Project
