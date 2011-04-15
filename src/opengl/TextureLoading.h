#ifndef PROJECT_OPENGL__TEXTURE_LOADING_H
#define PROJECT_OPENGL__TEXTURE_LOADING_H

#include "opengl/OpenGL.h"
#include <string>

namespace Project {
namespace OpenGL {

class TextureLoading {
public:

	/** Load a texture into openGL from the file @a filename
		Returns the texture id of the loaded texture, 0 if unsuccessful
	*/
	static GLuint loadTexture2D(std::string filename,
		GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT,
		GLint filter_min = GL_LINEAR_MIPMAP_LINEAR, GLint filter_mag = GL_LINEAR, bool generate_mipmaps = true,
		int* width = NULL, int* height = NULL);

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

}  // namespace OpenGL
}  // namespace Project

#endif
