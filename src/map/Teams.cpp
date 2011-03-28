#include "Teams.h"

namespace Project {
namespace Map {

	std::string Teams::teamName(int teamID) {
		switch (teamID % 8) {
			case 0: return "Red";
			case 1: return "Green";
			case 2: return "Blue";
			case 3: return "Yellow";
			case 4: return "Purple";
			case 5: return "Pink";
			case 6: return "Teal";
			case 7: return "Indigo";
			default: return "Invalid Team";
		}

	}

	OpenGL::Color Teams::teamColor(int id) {
		switch (id % 8) {
			case 0: return OpenGL::Color::RED;
			case 1: return OpenGL::Color::GREEN;
			case 2: return OpenGL::Color::BLUE;
			case 3: return OpenGL::Color::YELLOW;
			case 4: return OpenGL::Color::PURPLE;
			case 5: return OpenGL::Color::PINK;
			case 6: return OpenGL::Color::TEAL;
			case 7: return OpenGL::Color::INDIGO;
			default : return OpenGL::Color::WHITE;
		}
		
	}

}  // namespace Map
}  // namespace Project
