#include "Texture.h"

namespace Project {
namespace Render {

	Texture::Texture(std::string _name, std::string color_map_filename, std::string normal_map_filename) {
		name = _name;
		colorMapSet = false;
		normalMapSet = false;
	}

	std::string Texture::getName() const {
		return name;
	}

	bool Texture::hasColorMap() const {
		return colorMapSet;
	}

	GLuint Texture::getColorMap() const {
		return colorMap;
	}

	bool Texture::hasNormalMap() const {
		return normalMapSet;
	}

	GLuint Texture::getNormalMap() const {
		return normalMap;
	}


}  // namespace Render
}  // namespace Project
