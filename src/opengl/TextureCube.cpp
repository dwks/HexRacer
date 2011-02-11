#include "TextureCube.h"
#include "TextureLoading.h"

namespace Project {
namespace OpenGL {

	TextureCube::TextureCube(const CubeMapFile& file) {
		cubeMap = TextureLoading::loadTextureCube(
			file.getSideFile(0),
			file.getSideFile(1),
			file.getSideFile(2),
			file.getSideFile(3),
			file.getSideFile(4),
			file.getSideFile(5)
		);
	}

	TextureCube::TextureCube(std::string positive_x_file,
				std::string negative_x_file,
				std::string positive_y_file,
				std::string negative_y_file,
				std::string positive_z_file,
				std::string negative_z_file) {

					cubeMap = TextureLoading::loadTextureCube(positive_x_file,
						negative_x_file,
						positive_y_file,
						negative_y_file,
						positive_z_file,
						negative_z_file);


	}

	TextureCube::~TextureCube() {
		glDeleteTextures(1, &cubeMap);
	}

}  // namespace Render
}  // namespace Project
