#include "TextureCube.h"
#include "Texture.h"

namespace Project {
namespace Render {

	TextureCube::TextureCube(std::string positive_x_file,
				std::string negative_x_file,
				std::string positive_y_file,
				std::string negative_y_file,
				std::string positive_z_file,
				std::string negative_z_file) {

					cubeMap = Texture::loadTextureCube(positive_x_file,
						negative_x_file,
						positive_y_file,
						negative_y_file,
						positive_z_file,
						negative_z_file);

					int b = 5;


	}

	TextureCube::~TextureCube() {

	}

}  // namespace Render
}  // namespace Project
