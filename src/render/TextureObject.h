#ifndef PROJECT_RENDER__TEXTURE_OBJECT_H
#define PROJECT_RENDER__TEXTURE_OBJECT_H

#include "opengl/OpenGL.h"

namespace Project {
namespace Render {

class TextureObject {
public:

	enum TexCoordCorner { BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT };

	virtual GLuint getTextureID() const = 0;
	virtual bool hasTexture() const { return getTextureID() > 0; }

	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	virtual bool getHFlip() const { return false; }
	virtual bool getVFlip() const { return false; }

	virtual void glApplyCorner(TexCoordCorner corner) const {
		switch (corner) {
			case BOTTOM_LEFT: glTexCoord2i(0+(int)getHFlip(), 0+(int)getVFlip()); break;
			case BOTTOM_RIGHT: glTexCoord2i(1-(int)getHFlip(), 0+(int)getVFlip()); break;
			case TOP_LEFT: glTexCoord2i(0+(int)getHFlip(), 1-(int)getVFlip()); break;
			case TOP_RIGHT: glTexCoord2i(1-(int)getHFlip(), 1-(int)getVFlip()); break;
		}
	}
};

}  // namespace Render
}  // namespace Project

#endif
