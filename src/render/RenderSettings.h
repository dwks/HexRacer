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
	
	bool shadowMapping;
	int shadowMapWidth;
	int shadowMapHeight;

	bool redrawMode;
	bool allowDisplayLists;
	bool applyToShadowMatrix;

public:

	static const short GRAPHICS_HIGH = 3;
	static const short GRAPHICS_MED = 2;
	static const short GRAPHICS_LOW = 1;
	static const short GRAPHICS_VERY_LOW = 0;

	RenderSettings();

	//Config file set constants
	short getGraphicsQuality() const { return graphicsQuality; }
	short getMaxLights() const;
	bool getBloomEnabled() const { return bloomLighting; }
	int getBloomWidth() const { return bloomWidth; }
	int getBloomHeight() const { return bloomHeight; }
	bool getShadowMappingEnabled() const { return shadowMapping; }
	int getShadowMapWidth() const { return shadowMapWidth; }
	int getShadowMapHeight() const { return shadowMapHeight; }

	//Rendering modes changeable before rendering
	void setRedrawMode(bool redraw) { redrawMode = redraw; }
	bool getRedrawMode() const { return redrawMode; }

	void setApplyToShadowMatrix(bool apply) { applyToShadowMatrix = apply; }
	bool getApplyToShadowMatrix() const { return applyToShadowMatrix; }

	bool getAllowDisplayLists() const { return allowDisplayLists; }
	void setAllowDisplayLists(bool allow) { allowDisplayLists = allow; }
	
};

}  // namespace Render
}  // namespace Project

#endif
