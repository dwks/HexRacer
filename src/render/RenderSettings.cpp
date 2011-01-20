#include "RenderSettings.h"
#include "log/Logger.h"
#include <fstream>
using namespace std;

namespace Project {
namespace Render {

	RenderSettings *RenderSettings::instance = 0;

	RenderSettings::RenderSettings(const char* config_file_name) {
		instance = this;
		graphicsQuality = 2;

		ifstream in_file(config_file_name);
		if ((bool)in_file) {
			LOG(OPENGL, "Loading Render Config File: " << config_file_name);
			while (!in_file.eof()) {
				string var_name;
				in_file >> var_name;
				if (var_name == "graphicsQuality") {
					in_file.ignore(100, '=');
					in_file >> graphicsQuality;
					LOG(OPENGL, "	Setting Graphics Quality to " << graphicsQuality);
				}
			}
		}
		in_file.close();
	}

}  // namespace Render
}  // namespace Project
