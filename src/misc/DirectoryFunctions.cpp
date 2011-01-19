#include "DirectoryFunctions.h"
#include "math/Values.h"
#include <fstream>
using namespace Project;
using namespace Math;
using namespace std;

namespace Project {
namespace Misc {

	string DirectoryFunctions::extractFilename(string str) {
		std::string::size_type slashpos = str.find_last_of('/');
		if (slashpos == str.npos || slashpos+1 == str.length())
			return str;
		else {
			return str.substr(slashpos+1, str.length()-slashpos-1);
		}
	}

	string DirectoryFunctions::extractDirectory(string str) {
		return (str.substr(0,
			maximum<std::string::size_type>(str.find_last_of('/')+1, 0)));
	}

	bool DirectoryFunctions::fileExists(const char *name) {
		ifstream file(name);
		bool exists = (bool)file;
		file.close();
		return exists;
	}

}  // namespace Misc
}  // namespace Project
