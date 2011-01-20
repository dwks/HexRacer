#ifndef PROJECT_RENDER__RENDER_SETTINGS_H
#define PROJECT_RENDER__RENDER_SETTINGS_H

namespace Project {
namespace Render {

class RenderSettings {
private:
	static RenderSettings *instance;
	short graphicsQuality;
public:

	static RenderSettings *getInstance() { return instance; }
	RenderSettings(const char* config_file_name);
	short getGraphicsQuality() { return graphicsQuality; }
};

}  // namespace Render
}  // namespace Project

#endif
