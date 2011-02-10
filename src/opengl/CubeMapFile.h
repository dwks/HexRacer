#ifndef PROJECT_RENDER__CUBE_MAP_FILE_H
#define PROJECT_RENDER__CUBE_MAP_FILE_H

#include <string>

namespace Project {
namespace OpenGL {

class CubeMapFile {

private:

	std::string files[6];

public:

	CubeMapFile(std::string positive_x_file = "",
				std::string negative_x_file = "",
				std::string positive_y_file = "",
				std::string negative_y_file = "",
				std::string positive_z_file = "",
				std::string negative_z_file = "");

	std::string getSideFile(int index) const { return files[index]; }
	void setSideFile(int index, std::string filename) { files[index] = filename; }
	void clear();
	void setToFile(const CubeMapFile& file);
	static std::string getSideName(int index);
	static std::string getSideTitle(int index);
};

}  // namespace Render
}  // namespace Project

#endif
