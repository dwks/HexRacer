#ifndef PROJECT_RENDER__TEXTURE_H
#define PROJECT_RENDER__TEXTURE_H

#include "opengl/OpenGL.h"
#include <string>

namespace Project {
namespace Render {

/** Represents a texture made up of multiple images
*/
class Texture {

private:

	std::string name;
	GLuint colorMap;
	GLuint normalMap;
	GLuint glowMap;

public:

	Texture(std::string _name,
		std::string color_map_filename,
		std::string normal_map_filename,
		std::string glow_map_filename);
	~Texture();

	std::string getName() const;

	bool hasColorMap() const;
	GLuint getColorMap() const;
	bool hasNormalMap() const;
	GLuint getNormalMap() const;
	bool hasGlowMap() const;
	GLuint getGlowMap() const;
	//virtual GLenum targetTexture() { return GL_TEXTURE_2D; }

	/** Load a texture into openGL from the file @a filename
		Returns the texture id of the loaded texture, 0 if unsuccessful
	*/
	static GLuint loadTexture2D(std::string filename,
		GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT,
		GLint filter_min = GL_LINEAR_MIPMAP_LINEAR, GLint filter_mag = GL_LINEAR, bool generate_mipmaps = true);

	//static void loadSurfaceToTexture(SDLSurface* surface, GLenum target);

	static GLuint loadTextureCube(
		std::string positive_x_file,
		std::string negative_x_file,
		std::string positive_y_file,
		std::string negative_y_file,
		std::string positive_z_file,
		std::string negative_z_file,
		GLint filter_min = GL_LINEAR_MIPMAP_LINEAR, GLint filter_mag = GL_LINEAR);

	static GLint toGLFormat(int bytes_per_pixel);


};

}  // namespace Render
}  // namespace Project

#endif
