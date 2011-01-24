#include "ColorConstants.h"

namespace Project {
namespace Render {

	OpenGL::Color ColorConstants::playerColor(int id) {
		switch (id % 8) {
			case 0: return OpenGL::Color::RED;
			case 1: return OpenGL::Color::GREEN;
			case 2: return OpenGL::Color::BLUE;
			case 3: return OpenGL::Color::YELLOW;
			case 4: return OpenGL::Color::ORANGE;
			case 5: return OpenGL::Color::PURPLE;
			case 6: return OpenGL::Color::TEAL;
			case 7: return OpenGL::Color::INDIGO;
		}
		return OpenGL::Color::WHITE;
	}

}  // namespace Render
}  // namespace Project
