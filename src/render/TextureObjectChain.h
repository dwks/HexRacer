#ifndef PROJECT_RENDER__TEXTURE_OBJECT_CHAIN_H
#define PROJECT_RENDER__TEXTURE_OBJECT_CHAIN_H

#include "TextureObject.h"
#include "math/BoundingBox2D.h"
#include <vector>

namespace Project {
namespace Render {

class TextureObjectChain {
public:
	enum AlignmentStyle {MIN, MID, MAX};
	enum Orientation {HORIZONTAL, VERTICAL};

private:
	std::vector<TextureObject*> textureObjects;

	Orientation orientation;
	AlignmentStyle alignment;
	AlignmentStyle minorAlign;
	int padding;
	int width;
	int height;
	int numObjects;

public:

	TextureObjectChain(Orientation _orientation = HORIZONTAL,
		AlignmentStyle _alignment = MID,
		AlignmentStyle minor_align = MID,
		int _padding = 0);

	void update();

	void resize(unsigned int new_size);
	void append(TextureObject* object);
	void append(const std::vector<TextureObject*>& objects);
	
	void setObject(int index, TextureObject* object);
	void removeObject(TextureObject* object);
	void removeObject(int index);
	void clear();
	unsigned int getNumSlots() const { return textureObjects.size(); }

	void setOrientation(Orientation _orientation) { orientation = _orientation; update(); }
	void setAlignment(AlignmentStyle align) { alignment = align;}
	void setMinorAlign(AlignmentStyle align) { minorAlign = align; }

	TextureObject* getObject(int index);
	
	void render(double x, double y, double minor_length);
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	int getNumObjects() const { return numObjects; }
};

}  // namespace Render
}  // namespace Project

#endif
