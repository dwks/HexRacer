#include "RenderSettings.h"
#include "log/Logger.h"
#include <fstream>
using namespace std;

namespace Project {
namespace Render {

	short RenderSettings::getMaxLights() {
		switch (getGraphicsQuality()) {
			case GRAPHICS_HIGH:
				return 8;
			case GRAPHICS_MED:
				return 2;
			case GRAPHICS_LOW:
				return 4;
			default:
				return 2;
		}
	}

	
}  // namespace Render
}  // namespace Project
