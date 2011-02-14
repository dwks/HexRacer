#include "RenderSettings.h"
#include "log/Logger.h"
#include <fstream>
using namespace std;

namespace Project {
namespace Render {

	RenderSettings::RenderSettings() {

		graphicsQuality = GET_SETTING("render.quality", 2);
		bloomLighting = GET_SETTING("render.bloom.enable", false);
		bloomWidth = GET_SETTING("render.bloom.width", 800);
		bloomHeight = GET_SETTING("render.bloom.height", 600);

		shadowMapping = GET_SETTING("render.shadow.enable", false);
		shadowMapWidth = GET_SETTING("render.shadow.width", 800);
		shadowMapHeight = GET_SETTING("render.shadow.height", 600);

		redrawMode = false;
		applyToShadowMatrix = false;

	}

	short RenderSettings::getMaxLights() {
		switch (getGraphicsQuality()) {
			case GRAPHICS_HIGH:
				return 8;
			case GRAPHICS_MED:
				return 4;
			case GRAPHICS_LOW:
				return 4;
			default:
				return 2;
		}
	}

	
}  // namespace Render
}  // namespace Project
