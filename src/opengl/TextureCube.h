#ifndef PROJECT_RENDER__TEXTURE_CUBE_H
#define PROJECT_RENDER__TEXTURE_CUBE_H

#include "opengl/OpenGL.h"
#include "opengl/CubeMapFile.h"
#include <string>

namespace Project {
namespace OpenGL {

class TextureCube {
private:
	GLuint cubeMap;
public:

	TextureCube(const CubeMapFile& file);

	TextureCube(std::string positive_x_file,
				std::string negative_x_file,
				std::string positive_y_file,
				std::string negative_y_file,
				std::string positive_z_file,
				std::string negative_z_file);

	~TextureCube();

	GLuint getCubeMap() const { return cubeMap; }
};

}  // namespace Render
}  // namespace Project

#endif
