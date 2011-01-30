#include "DirectoryFunctions.h"
#include "math/Values.h"
#include <fstream>
using namespace Project;
using namespace Math;
using namespace std;

namespace Project {
namespace Misc {

	bool DirectoryFunctions::fileExists(const char *name) {
		ifstream file(name);
		bool exists = (bool)file;
		file.close();
		return exists;
	}

	string DirectoryFunctions::extractFilename(string str, bool include_extension) {
		std::string::size_type slashpos = str.find_last_of('/');
		string return_str;

		if (slashpos == str.npos || slashpos+1 >= str.length())
			return_str = str;
		else {
			return_str = str.substr(slashpos+1, str.length()-slashpos-1);
		}

		std::string::size_type dotpos = return_str.find_last_of('.');

		if (include_extension || dotpos+1 >= str.length())
			return return_str;
		else 
			return return_str.substr(0, dotpos);
	}

	string DirectoryFunctions::extractDirectory(string str) {
		return (str.substr(0,
			maximum<std::string::size_type>(str.find_last_of('/')+1, 0)));
	}

	string DirectoryFunctions::toRelativeFilename(string base_directory, string full_filename) {
		if (full_filename.size() > base_directory.size() &&
			full_filename.substr(0, base_directory.size()) == base_directory) {
			return full_filename.substr(base_directory.length(), full_filename.length()-base_directory.length());
		}
		else {
			return full_filename;
		}
	}

	string DirectoryFunctions::fromRelativeFilename(string base_directory, string relative_filename) {
		return base_directory.append(relative_filename);
	}

}  // namespace Misc
}  // namespace Project
