#ifndef PROJECT_RENDER__TEXTURE_H
#define PROJECT_RENDER__TEXTURE_H

#include "opengl/OpenGL.h"
#include <string>

namespace Project {
namespace Render {

/** Represents a texture made up of multiple images
*/
class TexturePack {

private:

	std::string name;
	GLuint colorMap;
	GLuint normalMap;
	GLuint glowMap;

public:

	TexturePack(std::string _name,
		std::string color_map_filename,
		std::string normal_map_filename,
		std::string glow_map_filename);
	~TexturePack();

	const std::string& getName() const { return name; }

	bool hasColorMap() const { return colorMap > 0; }
	GLuint getColorMap() const { return colorMap; }
	bool hasNormalMap() const { return normalMap > 0; }
	GLuint getNormalMap() const { return normalMap; }
	bool hasGlowMap() const { return glowMap > 0; }
	GLuint getGlowMap() const { return glowMap; }

};

}  // namespace Render
}  // namespace Project

#endif
