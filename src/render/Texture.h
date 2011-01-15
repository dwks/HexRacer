#ifndef PROJECT_RENDER__TEXTURE_H
#define PROJECT_RENDER__TEXTURE_H

#include "opengl/OpenGL.h"
#include <string>

namespace Project {
namespace Render {

class Texture {

private:

	std::string name;
	GLuint colorMap;
	GLuint normalMap;

public:

	Texture(std::string _name, std::string color_map_filename, std::string normal_map_filename);

	std::string getName() const;

	bool hasColorMap() const;
	GLuint getColorMap() const;
	bool hasNormalMap() const;
	GLuint getNormalMap() const;

	static GLuint loadTexture(std::string filename,
		GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT,
		GLint filter_s = GL_LINEAR, GLint filter_t = GL_LINEAR);


};

}  // namespace Render
}  // namespace Project

#endif
