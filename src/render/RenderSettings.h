#ifndef PROJECT_RENDER__RENDER_SETTINGS_H
#define PROJECT_RENDER__RENDER_SETTINGS_H

#include "settings/SettingsManager.h"

namespace Project {
namespace Render {

class RenderSettings {
private:
	short graphicsQuality;
public:

	static const short GRAPHICS_HIGH = 3;
	static const short GRAPHICS_MED = 2;
	static const short GRAPHICS_LOW = 1;
	static const short GRAPHICS_VERY_LOW = 0;

	short getGraphicsQuality() { return GET_SETTING("render.quality", 2); }
	short getMaxLights();
};

}  // namespace Render
}  // namespace Project

#endif
