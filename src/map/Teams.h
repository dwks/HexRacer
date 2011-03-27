#ifndef PROJECT_MAP__TEAMS_H
#define PROJECT_MAP__TEAMS_H

#include <string>
#include "opengl/Color.h"

namespace Project {
namespace Map {

class Teams {
public:
	static const int MAX_TEAMS = 8;
	static std::string teamName(int teamID);
	static OpenGL::Color teamColor(int id);
};

}  // namespace Map
}  // namespace Project

#endif
