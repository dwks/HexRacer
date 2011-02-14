#ifndef PROJECT_RENDER__RENDER_SETTINGS_H
#define PROJECT_RENDER__RENDER_SETTINGS_H

#include "settings/SettingsManager.h"

namespace Project {
namespace Render {

class RenderSettings {
private:
	
	short graphicsQuality;
	bool bloomLighting;
	int bloomWidth;
	int bloomHeight;

	bool redrawMode;

public:

	static const short GRAPHICS_HIGH = 3;
	static const short GRAPHICS_MED = 2;
	static const short GRAPHICS_LOW = 1;
	static const short GRAPHICS_VERY_LOW = 0;

	RenderSettings();

	//Config file set constants
	short getGraphicsQuality() { return graphicsQuality; }
	short getMaxLights();
	bool getBloomEnabled() { return bloomLighting; }
	int getBloomWidth() { return bloomWidth; }
	int getBloomHeight() { return bloomHeight; }

	//Rendering modes changeable before rendering
	void setRedrawMode(bool redraw) { redrawMode = redraw; }
	bool getRedrawMode() const { return redrawMode; }
	
};

}  // namespace Render
}  // namespace Project

#endif
