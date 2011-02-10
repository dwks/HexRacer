#include "CubeMapFile.h"

namespace Project {
namespace OpenGL {

	CubeMapFile::CubeMapFile(std::string positive_x_file,
				std::string negative_x_file,
				std::string positive_y_file,
				std::string negative_y_file,
				std::string positive_z_file,
				std::string negative_z_file) {

		files[0] = positive_x_file;
		files[1] = negative_x_file;
		files[2] = positive_y_file;
		files[3] = negative_y_file;
		files[4] = positive_z_file;
		files[5] = negative_z_file;

	}

	std::string CubeMapFile::getSideName(int index) {
		switch (index) {
			case 0:
				return "cubeMapPosX";
			case 1:
				return "cubeMapNegX";
			case 2:
				return "cubeMapPosY";
			case 3:
				return "cubeMapNegY";
			case 4:
				return "cubeMapPosZ";
			case 5:
				return "cubeMapNegZ";
			default:
				return "";
		}
	}

	std::string CubeMapFile::getSideTitle(int index) {
		switch (index) {
			case 0:
				return "Positive X";
			case 1:
				return "Negative X";
			case 2:
				return "Positive Y";
			case 3:
				return "Negative Y";
			case 4:
				return "Positive Z";
			case 5:
				return "Negative Z";
			default:
				return "";
		}
	}

	void CubeMapFile::clear() {
		files[0] = "";
		files[1] = "";
		files[2] = "";
		files[3] = "";
		files[4] = "";
		files[5] = "";
	}

	void CubeMapFile::setToFile(const CubeMapFile& file) {
		files[0] = file.getSideFile(0);
		files[1] = file.getSideFile(1);
		files[2] = file.getSideFile(2);
		files[3] = file.getSideFile(3);
		files[4] = file.getSideFile(4);
		files[5] = file.getSideFile(5);
	}

}  // namespace Render
}  // namespace Project
