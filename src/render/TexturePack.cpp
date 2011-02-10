#include "TexturePack.h"
#include "opengl/TextureLoading.h"

namespace Project {
namespace Render {

	TexturePack::TexturePack(std::string _name,
		std::string color_map_filename,
		std::string normal_map_filename,
		std::string glow_map_filename) {
		name = _name;
		colorMap = OpenGL::TextureLoading::loadTexture2D(color_map_filename);
		normalMap = OpenGL::TextureLoading::loadTexture2D(normal_map_filename);
		glowMap = OpenGL::TextureLoading::loadTexture2D(glow_map_filename);
	}

	TexturePack::~TexturePack() {
		glDeleteTextures(1, &colorMap);
		glDeleteTextures(1, &normalMap);
		glDeleteTextures(1, &glowMap);
	}

}  // namespace Render
}  // namespace Project
