#ifndef PROJECT_RENDER__RENDER_SETTINGS_H
#define PROJECT_RENDER__RENDER_SETTINGS_H

#include "settings/SettingsManager.h"

namespace Project {
namespace Render {

class RenderSettings {
private:
	static RenderSettings *instance;
	short graphicsQuality;
public:

	static RenderSettings *getInstance() { return instance; }
	short getGraphicsQuality() { return GET_SETTING("render.quality", 2); }
};

}  // namespace Render
}  // namespace Project

#endif
