#include "PlayerPlacingText.h"
#include <sstream>
#include "render/StringTextureCache.h"
#include "render/FontManager.h"
#include "opengl/OpenGL.h"
#include "render/TextureObjectChain.h"

namespace Project {
namespace HUD {

	PlayerPlacingText::PlayerPlacingText() {
		placing =-1;
	}

	void PlayerPlacingText::setPlacing(int _placing) {

		if (placing != _placing) {
			placing = _placing;

			if (placing < 0)
				placingString = "";
			else {
			
				std::stringstream stream;
				stream << (placing+1);
				switch ((placing+1) % 10) {
					case 0: case 4: case 5: case 6: case 8: case 9:
						stream << "th"; break;
					case 1: stream << "st"; break;
					case 2: stream << "nd"; break;
					case 3: stream << "rd"; break;
				}

				placingString = stream.str();

			}
		}

	}

	void PlayerPlacingText::render() {
		if (placing < 0)
			return;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		Render::StringTexture* texture = Render::StringTextureCache::getInstance()->getStringTexture(
			placingString,
			Render::FontManager::getInstance()->getCurrentFont()
			);

		double height = (double) texture->getHeight() / (double) texture->getWidth();

		OpenGL::Color::glColor(OpenGL::Color::WHITE);
		glEnable(GL_TEXTURE_2D);

		Render::TextureObjectChain chain;
		chain.append(texture);
		chain.render(0.5, 0.5, height);

	}

}  // namespace HUD
}  // namespace Project
