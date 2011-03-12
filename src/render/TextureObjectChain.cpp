#include "TextureObjectChain.h"
#include "misc/StdVectorFunctions.h"
#include "math/Values.h"

namespace Project {
namespace Render {

	TextureObjectChain::TextureObjectChain(TextureObjectChain::Orientation _orientation,
		TextureObjectChain::AlignmentStyle _alignment,
		TextureObjectChain::AlignmentStyle minor_align,
		int _padding)
		: orientation(_orientation), alignment(_alignment), minorAlign(minor_align), padding(_padding) {
	}

	void TextureObjectChain::resize(unsigned int new_size) {

		int size_difference = new_size - textureObjects.size();

		if (size_difference > 0) {
			for (int i = 0; i < size_difference; i++)
				textureObjects.push_back(NULL);
		}
		else if (size_difference < 0) {
			textureObjects.resize(new_size);
			update();
		}
		
	}

	void TextureObjectChain::append(TextureObject* object) {
		textureObjects.push_back(object);
		update();
	}
	void TextureObjectChain::append(const std::vector<TextureObject*>& objects) {
		Misc::vectorAppend(textureObjects, objects);
		update();
	}
	void TextureObjectChain::setObject(int index, TextureObject* object) {
		textureObjects[index] = object;
		update();
	}

	void TextureObjectChain::removeObject(TextureObject* object) {
		Misc::vectorRemoveOneElement(textureObjects, object);
		update();
	}

	void TextureObjectChain::removeObject(int index) {
		Misc::vectorRemoveAtIndex(textureObjects, index);
		update();
	}

	void TextureObjectChain::clear() {
		textureObjects.clear();
		update();
	}
	TextureObject* TextureObjectChain::getObject(int index) {
		return textureObjects[index];
	}

	void TextureObjectChain::update() {

		width = 0;
		height = 0;
		numObjects = 0;

		for (unsigned int i = 0; i < textureObjects.size(); i++) {
			TextureObject* object = textureObjects[i];

			if (object && object->hasTexture()) {
				if (orientation == HORIZONTAL) {
					width += std::abs(object->getWidth());
					height = Math::maximum(height, std::abs(object->getHeight()));
				}
				else {
					width = Math::maximum(width, std::abs(object->getWidth()));
					height += std::abs(object->getHeight());
				}
				numObjects++;
			}
		}

		if (orientation == HORIZONTAL)
			width += (numObjects-1)*padding;
		else
			height += (numObjects-1)*padding;
	}

	void TextureObjectChain::render(double x, double y, double minor_length) {

		double u;
		double v;
		double major_length;
		double padding_length;
		
		if (orientation == HORIZONTAL) {

			major_length = minor_length * (double) width / (double) height;

			padding_length = (double) padding / (double) width * major_length;

			switch (alignment) {
				case MIN: u = x; break;
				case MID: u = x-(major_length)/2.0; break;
				case MAX: u = x-major_length; break;
			}

			switch (minorAlign) {
				case MIN: v = y; break;
				case MID: v = y-minor_length/2.0; break;
				case MAX: v = y-minor_length;  break;
			}

		}
		else {

			major_length = minor_length * (double) height / (double) width;

			padding_length = (double) padding / (double) height * major_length;

			switch (alignment) {
				case MIN: v = y; break;
				case MID: v = y-(major_length)/2.0; break;
				case MAX: v = y-major_length; break;
			}

			switch (minorAlign) {
				case MIN: u = x; break;
				case MID: u = x-minor_length/2.0; break;
				case MAX: u = x-minor_length;  break;
			}

		}


		for (unsigned int i = 0; i < textureObjects.size(); i++) {

			TextureObject* object = textureObjects[i];
			if (object && object->hasTexture()) {

				double w = (double) object->getWidth() / (double) width;
				double h = (double) object->getHeight() / (double) height;

				if (orientation == HORIZONTAL) {
					w *= major_length;
					h *= minor_length;
				}
				else {
					w *= minor_length;
					h *= major_length;
				}

				glBindTexture(GL_TEXTURE_2D, object->getTextureID());

				glBegin(GL_QUADS);
				object->glApplyCorner(TextureObject::BOTTOM_LEFT);
				glVertex2d(u, v);
				object->glApplyCorner(TextureObject::BOTTOM_RIGHT);
				glVertex2d(u+w, v);
				object->glApplyCorner(TextureObject::TOP_RIGHT);
				glVertex2d(u+w, v+h);
				object->glApplyCorner(TextureObject::TOP_LEFT);
				glVertex2d(u, v+h);
				glEnd();

				if (orientation == HORIZONTAL) {
					u += w + padding_length;
				}
				else {
					v += h + padding_length;
				}

			}
		}

	}


}  // namespace Render
}  // namespace Project
