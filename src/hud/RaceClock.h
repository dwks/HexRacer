#ifndef PROJECT_HUD__RACE_CLOCK_H
#define PROJECT_HUD__RACE_CLOCK_H

#include "render/TextureObjectChain.h"
#include "render/StringTexture.h"

namespace Project {
namespace HUD {

class RaceClock {
private:
	int width;
	int height;
	Render::TextureObjectChain entry;
	Render::StringTexture timeTexture;
public:
	RaceClock();
	void setWidth(int _width) { width = _width; }
	void setHeight(int _height) { height = _height; }
	void setSeconds(unsigned int seconds);
	void render();
};

}  // namespace HUD
}  // namespace Project

#endif
