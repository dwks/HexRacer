#ifndef PROJECT_RENDER__TEXTURE_H
#define PROJECT_RENDER__TEXTURE_H

#include "opengl/OpenGL.h"
#include <string>

namespace Project {
namespace Render {

class Texture {

private:

	std::string name;
	bool colorMapSet;
	bool normalMapSet;
	GLuint colorMap;
	GLuint normalMap;


public:

	Texture(std::string _name, std::string color_map_filename, std::string normal_map_filename);

	std::string getName() const;

	bool hasColorMap() const;
	GLuint getColorMap() const;
	bool hasNormalMap() const;
	GLuint getNormalMap() const;


};

}  // namespace Render
}  // namespace Project

#endif
