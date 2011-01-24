#ifndef PROJECT_RENDER__COLOR_CONSTANTS_H
#define PROJECT_RENDER__COLOR_CONSTANTS_H

#include "opengl/Color.h"

namespace Project {
namespace Render {

class ColorConstants {
public:
	static OpenGL::Color playerColor(int id);
};

}  // namespace Render
}  // namespace Project

#endif
